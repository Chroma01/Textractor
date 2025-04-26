#include "qtcommon.h"
#include "translatewrapperdeeplx.h"
#include "network.h"

extern const wchar_t *TRANSLATION_ERROR;

const char *TRANSLATION_PROVIDER = "DeepLX Translate";
const char *GET_API_KEY_FROM = "https://deeplx.owo.network/install/variables.html";

extern const QStringList version{
    "free", "v1", "v2"
};
extern const QStringList languagesTo
        {
            "Arabic",
            "Bulgarian",
            "Czech",
            "Danish",
            "German",
            "Greek",
            "English (backward compatibility)",
            "English (British)",
            "English (American)",
            "Spanish",
            "Estonian",
            "Finnish",
            "French",
            "Hungarian",
            "Indonesian",
            "Italian",
            "Japanese",
            "Korean",
            "Lithuanian",
            "Latvian",
            "Norwegian Bokm�l",
            "Dutch",
            "Polish",
            "Portuguese (backward compatibility)",
            "Portuguese (Brazilian)",
            "Portuguese (all Portuguese variants excluding Brazilian Portuguese)",
            "Romanian",
            "Russian",
            "Slovak",
            "Slovenian",
            "Swedish",
            "Turkish",
            "Ukrainian",
            "Chinese (backward compatibility)",
            "Chinese (simplified)",
            "Chinese (traditional)"
        },
        languagesFrom
        {
            "Arabic",
            "Bulgarian",
            "Chinese (all Chinese variants)",
            "Czech",
            "Danish",
            "Dutch",
            "English (all English variants)",
            "Estonian",
            "Finnish",
            "French",
            "German",
            "Greek",
            "Hungarian",
            "Indonesian",
            "Italian",
            "Japanese",
            "Korean",
            "Latvian",
            "Lithuanian",
            "Norwegian Bokm�l",
            "Polish",
            "Portuguese (all Portuguese variants)",
            "Romanian",
            "Russian",
            "Slovak",
            "Slovenian",
            "Spanish",
            "Swedish",
            "Turkish",
            "Ukrainian"
        };
extern const std::unordered_map<std::wstring, std::wstring> codes
{
    {{L"Arabic"}, {L"AR"}},
    {{L"Bulgarian"}, {L"BG"}},
    {{L"Czech"}, {L"CS"}},
    {{L"Danish"}, {L"DA"}},
    {{L"German"}, {L"DE"}},
    {{L"Greek"}, {L"EL"}},
    {{L"English (all English variants)"}, {L"EN"}},
    {{L"English (backward compatibility)"}, {L"EN"}},
    {{L"English (British)"}, {L"EN-GB"}},
    {{L"English (American)"}, {L"EN-US"}},
    {{L"Spanish"}, {L"ES"}},
    {{L"Estonian"}, {L"ET"}},
    {{L"Finnish"}, {L"FI"}},
    {{L"French"}, {L"FR"}},
    {{L"Hungarian"}, {L"HU"}},
    {{L"Indonesian"}, {L"ID"}},
    {{L"Italian"}, {L"IT"}},
    {{L"Japanese"}, {L"JA"}},
    {{L"Korean"}, {L"KO"}},
    {{L"Lithuanian"}, {L"LT"}},
    {{L"Latvian"}, {L"LV"}},
    {{L"Norwegian Bokm�l"}, {L"NB"}},
    {{L"Dutch"}, {L"NL"}},
    {{L"Polish"}, {L"PL"}},
    {{L"Portuguese (all Portuguese variants)"}, {L"PT"}},
    {{L"Portuguese (backward compatibility)"}, {L"PT"}},
    {{L"Portuguese (Brazilian)"}, {L"PT-BR"}},
    {{L"Portuguese (all Portuguese variants excluding Brazilian Portuguese)"}, {L"PT-PT"}},
    {{L"Romanian"}, {L"RO"}},
    {{L"Russian"}, {L"RU"}},
    {{L"Slovak"}, {L"SK"}},
    {{L"Slovenian"}, {L"SL"}},
    {{L"Swedish"}, {L"SV"}},
    {{L"Turkish"}, {L"TR"}},
    {{L"Ukrainian"}, {L"UK"}},
    {{L"Chinese (all Chinese variants)"}, {L"ZH"}},
    {{L"Chinese (backward compatibility)"}, {L"ZH"}},
    {{L"Chinese (simplified)"}, {L"ZH-HANS"}},
    {{L"Chinese (traditional)"}, {L"ZH-HANT"}},
    {{L"?"}, {L"auto"}}

};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 5000, port = 1188;

std::wstring RemoveZeroWidthSpace(std::wstring text) {
    text.erase(std::remove(text.begin(), text.end(), 0x200b), text.end());
    return text;
}

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
    std::wstring cleanedText = RemoveZeroWidthSpace(text);
    if (tlp.translateVersion == L"free") {
        if (HttpRequest httpRequest{
            L"Mozilla/5.0 Textractor",
            L"127.0.0.1",
            L"POST",
            L"/translate",
            FormatString(R"({"text":"%s","source_lang":"%S","target_lang":"%S"})",
                         JSON::Escape(WideStringToString(cleanedText)), codes.at(tlp.translateFrom),
                         codes.at(tlp.translateTo)),
            FormatString(L"Content-Type: application/json; charset=UTF-8\r\nAuthorization: Bearer %s",
                         tlp.authKey).c_str(),
            DWORD(port),
            NULL,
            WINHTTP_FLAG_ESCAPE_DISABLE
        }) {
            if (auto translation = Copy(JSON::Parse(httpRequest.response)[L"data"].String()))
                return {true, HTML::Unescape(translation.value())};
            return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
        } else return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
    } else if (tlp.translateVersion == L"v1") {
        if (HttpRequest httpRequest{
            L"Mozilla/5.0 Textractor",
            L"127.0.0.1",
            L"POST",
            L"/v1/translate",
            FormatString(R"({"text":"%s","source_lang":"%S","target_lang":"%S"})",
                         JSON::Escape(WideStringToString(text)), codes.at(tlp.translateFrom),
                         codes.at(tlp.translateTo)),
            FormatString(L"Content-Type: application/json; charset=UTF-8\r\nAuthorization: Bearer %s",
                         tlp.authKey).c_str(),
            DWORD(port),
            NULL,
            WINHTTP_FLAG_ESCAPE_DISABLE
        }) {
            if (auto translation = Copy(JSON::Parse(httpRequest.response)[L"data"].String()))
                return {true, HTML::Unescape(translation.value())};
            return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
        } else return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
    } else {
        // v2
        if (HttpRequest httpRequest{
            L"Mozilla/5.0 Textractor",
            L"127.0.0.1",
            L"POST",
            L"/v2/translate",
            FormatString(R"({"text":["%s"],"source_lang":"%S","target_lang":"%S"})",
                         JSON::Escape(WideStringToString(text)), codes.at(tlp.translateFrom),
                         codes.at(tlp.translateTo)),
            FormatString(L"Content-Type: application/json; charset=UTF-8\r\nAuthorization: DeepL-Auth-Key %s",
                         tlp.authKey).c_str(),
            DWORD(port),
            NULL,
            WINHTTP_FLAG_ESCAPE_DISABLE
        }) {
            if (auto translation = Copy(JSON::Parse(httpRequest.response)[L"translations"][0][L"text"].String()))
                return {true, HTML::Unescape(translation.value())};
            return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
        } else return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
    }
}
