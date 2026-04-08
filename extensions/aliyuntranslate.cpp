#include "qtcommon.h"
#include "translatewrapperaliyun.h"
#include "network.h"
#include "hashutils.h"
#include <nlohmann/json.hpp>
#include <ctime>
#include <atomic>

using json = nlohmann::json;

extern const wchar_t *TRANSLATION_ERROR;

const char *TRANSLATION_PROVIDER = "Aliyun Translate";
const char *GET_API_KEY_FROM =
        "https://help.aliyun.com/zh/machine-translation";

extern const QStringList version{
    "free", "general", "pro"
};
extern const QStringList languagesTo
        {
            "Abkhazian",
            "Achinese",
            "Afrikaans",
            "Akan",
            "Albanian",
            "Amharic",
            "Arabic",
            "Aragonese",
            "Armenian",
            "Assamese",
            "Asturian",
            "Aymara",
            "Azerbaijani",
            "Baluchi",
            "Bambara",
            "Bashkir",
            "Basque",
            "Belarusian",
            "Bemba (Zambia)",
            "Bengali",
            "Berber languages",
            "Bhojpuri",
            "Bislama",
            "Blin",
            "Bosnian",
            "Breton",
            "Bulgarian",
            "Burmese",
            "Cantonese",
            "Catalan",
            "Cebuano",
            "Central Huasteca Nahuatl",
            "Chamorro",
            "Chavacano",
            "Cherokee",
            "Chuvash",
            "Cornish",
            "Corsican",
            "Creek",
            "Crimean Tatar",
            "Czech",
            "Danish",
            "Dhivehi",
            "Dutch",
            "English",
            "Esperanto",
            "Estonian",
            "Ewe",
            "Faroese",
            "Fijian",
            "Filipino",
            "Finnish",
            "French",
            "Friulian",
            "Fur",
            "Galician",
            "Ganda",
            "Georgian",
            "German",
            "Gilbertese",
            "Gronings",
            "Guarani",
            "Guerrero Nahuatl",
            "Gujarati",
            "Haitian",
            "Hausa",
            "Hawaiian",
            "Hebrew",
            "Hiligaynon",
            "Hindi",
            "Hmong",
            "Hungarian",
            "Hupa",
            "Iban",
            "Icelandic",
            "Ido",
            "Igbo",
            "Iloko",
            "Indonesia",
            "Ingush",
            "Interlingua",
            "Interlingue",
            "Inuktitut",
            "Irish",
            "Italian",
            "Japanese",
            "Javanese",
            "Kabyle",
            "Kadazan Dusun",
            "Kalaallisut",
            "Kam",
            "Kannada",
            "Kanuri",
            "Kashmiri",
            "Kashubian",
            "Kazakh",
            "Kekchi",
            "Khasi",
            "Khmer",
            "Kiche",
            "Kinyarwanda",
            "Kirghiz",
            "Klingon",
            "Kongo",
            "Korean",
            "Kurdish",
            "Lao",
            "Latgalian",
            "Latin",
            "Latvian",
            "Letzeburgesch",
            "Limburgish",
            "Lingala",
            "Lingua Franca Nova",
            "Lithuanian",
            "Lojban",
            "Low German",
            "Macedonian",
            "Maithili",
            "Malagasy",
            "Malay",
            "Malayalam",
            "Maltese",
            "Manx",
            "Maori",
            "Marathi",
            "Mari (Russia)",
            "Marshallese",
            "Modern Greek",
            "Mongolian",
            "Montenegrin",
            "Morisyen",
            "Navajo",
            "Nepali",
            "Niuean",
            "Northern Sami",
            "Norwegian",
            "Nyanja",
            "Occitan",
            "Ojibwa",
            "Oriya",
            "Oromo",
            "Ossetian",
            "Pam",
            "Pangasinan",
            "Panjabi",
            "Papiamento",
            "Persian",
            "Polish",
            "Portuguese",
            "Pushto",
            "Quechua",
            "Romanian",
            "Romansh",
            "Romany",
            "Rundi",
            "Russian",
            "Rusyn",
            "Samoan",
            "Sango",
            "Sanskrit",
            "Scots",
            "Serbo-Croatian",
            "Shan",
            "Shona",
            "Silesian",
            "Simplified Chinese",
            "Sindhi",
            "Sinhala",
            "Slovak",
            "Slovenian",
            "Somali",
            "Spanish",
            "Sundanese",
            "Swahili",
            "Swedish",
            "Syriac",
            "Tagalog",
            "Tahitian",
            "Tajik",
            "Tamil",
            "Tatar",
            "Telugu",
            "Tetum",
            "Thai",
            "Tigrinya",
            "Tok Pisin",
            "Tonga (Tonga Islands)",
            "Tonga (Zambia)",
            "Traditional Chinese",
            "Tsonga",
            "Turkish",
            "Turkmen",
            "Tuvalu",
            "Tuvinian",
            "Twi",
            "Udmurt",
            "Umbundu",
            "Upper Sorbian",
            "Urdu",
            "Uzbek",
            "Venda",
            "Vietnamese",
            "Volapuk",
            "Walloon",
            "Waray (Philippines)",
            "Welsh",
            "Western Frisian",
            "Wolof",
            "Xhosa",
            "Yiddish",
            "Yoruba",
            "Zaza",
            "Zulu",
        }, languagesFrom = languagesTo;
extern const std::unordered_map<std::wstring, std::wstring> codes
{
    {{L"Abkhazian"}, {L"ab"}},
    {{L"Achinese"}, {L"ace"}},
    {{L"Afrikaans"}, {L"af"}},
    {{L"Akan"}, {L"ak"}},
    {{L"Albanian"}, {L"sq"}},
    {{L"Amharic"}, {L"am"}},
    {{L"Arabic"}, {L"ar"}},
    {{L"Aragonese"}, {L"an"}},
    {{L"Armenian"}, {L"hy"}},
    {{L"Assamese"}, {L"as"}},
    {{L"Asturian"}, {L"ast"}},
    {{L"Aymara"}, {L"ay"}},
    {{L"Azerbaijani"}, {L"az"}},
    {{L"Baluchi"}, {L"bal"}},
    {{L"Bambara"}, {L"bm"}},
    {{L"Bashkir"}, {L"ba"}},
    {{L"Basque"}, {L"eu"}},
    {{L"Belarusian"}, {L"be"}},
    {{L"Bemba (Zambia)"}, {L"bem"}},
    {{L"Bengali"}, {L"bn"}},
    {{L"Berber languages"}, {L"ber"}},
    {{L"Bhojpuri"}, {L"bho"}},
    {{L"Bislama"}, {L"bi"}},
    {{L"Blin"}, {L"byn"}},
    {{L"Bosnian"}, {L"bs"}},
    {{L"Breton"}, {L"br"}},
    {{L"Bulgarian"}, {L"bg"}},
    {{L"Burmese"}, {L"my"}},
    {{L"Cantonese"}, {L"yue"}},
    {{L"Catalan"}, {L"ca"}},
    {{L"Cebuano"}, {L"ceb"}},
    {{L"Central Huasteca Nahuatl"}, {L"nch"}},
    {{L"Chamorro"}, {L"ch"}},
    {{L"Chavacano"}, {L"cbk"}},
    {{L"Cherokee"}, {L"chr"}},
    {{L"Chuvash"}, {L"cv"}},
    {{L"Cornish"}, {L"kw"}},
    {{L"Corsican"}, {L"co"}},
    {{L"Creek"}, {L"mus"}},
    {{L"Crimean Tatar"}, {L"crh"}},
    {{L"Czech"}, {L"cs"}},
    {{L"Danish"}, {L"da"}},
    {{L"Dhivehi"}, {L"dv"}},
    {{L"Dutch"}, {L"nl"}},
    {{L"English"}, {L"en"}},
    {{L"Esperanto"}, {L"eo"}},
    {{L"Estonian"}, {L"et"}},
    {{L"Ewe"}, {L"ee"}},
    {{L"Faroese"}, {L"fo"}},
    {{L"Fijian"}, {L"fj"}},
    {{L"Filipino"}, {L"fil"}},
    {{L"Finnish"}, {L"fi"}},
    {{L"French"}, {L"fr"}},
    {{L"Friulian"}, {L"fur"}},
    {{L"Fur"}, {L"fvr"}},
    {{L"Galician"}, {L"gl"}},
    {{L"Ganda"}, {L"lg"}},
    {{L"Georgian"}, {L"ka"}},
    {{L"German"}, {L"de"}},
    {{L"Gilbertese"}, {L"gil"}},
    {{L"Gronings"}, {L"gos"}},
    {{L"Guarani"}, {L"gn"}},
    {{L"Guerrero Nahuatl"}, {L"ngu"}},
    {{L"Gujarati"}, {L"gu"}},
    {{L"Haitian"}, {L"ht"}},
    {{L"Hausa"}, {L"ha"}},
    {{L"Hawaiian"}, {L"haw"}},
    {{L"Hebrew"}, {L"he"}},
    {{L"Hiligaynon"}, {L"hil"}},
    {{L"Hindi"}, {L"hi"}},
    {{L"Hmong"}, {L"hmn"}},
    {{L"Hungarian"}, {L"hu"}},
    {{L"Hupa"}, {L"hup"}},
    {{L"Iban"}, {L"iba"}},
    {{L"Icelandic"}, {L"is"}},
    {{L"Ido"}, {L"io"}},
    {{L"Igbo"}, {L"ig"}},
    {{L"Iloko"}, {L"ilo"}},
    {{L"Indonesia"}, {L"id"}},
    {{L"Ingush"}, {L"inh"}},
    {{L"Interlingua"}, {L"ia"}},
    {{L"Interlingue"}, {L"ie"}},
    {{L"Inuktitut"}, {L"iu"}},
    {{L"Irish"}, {L"ga"}},
    {{L"Italian"}, {L"it"}},
    {{L"Japanese"}, {L"ja"}},
    {{L"Javanese"}, {L"jv"}},
    {{L"Kabyle"}, {L"kab"}},
    {{L"Kadazan Dusun"}, {L"dtp"}},
    {{L"Kalaallisut"}, {L"kl"}},
    {{L"Kam"}, {L"kdx"}},
    {{L"Kannada"}, {L"kn"}},
    {{L"Kanuri"}, {L"kr"}},
    {{L"Kashmiri"}, {L"ks"}},
    {{L"Kashubian"}, {L"csb"}},
    {{L"Kazakh"}, {L"kk"}},
    {{L"Kekchi"}, {L"kek"}},
    {{L"Khasi"}, {L"kha"}},
    {{L"Khmer"}, {L"km"}},
    {{L"Kiche"}, {L"quc"}},
    {{L"Kinyarwanda"}, {L"rw"}},
    {{L"Kirghiz"}, {L"ky"}},
    {{L"Klingon"}, {L"tlh"}},
    {{L"Kongo"}, {L"kg"}},
    {{L"Korean"}, {L"ko"}},
    {{L"Kurdish"}, {L"ku"}},
    {{L"Lao"}, {L"lo"}},
    {{L"Latgalian"}, {L"ltg"}},
    {{L"Latin"}, {L"la"}},
    {{L"Latvian"}, {L"lv"}},
    {{L"Letzeburgesch"}, {L"lb"}},
    {{L"Limburgish"}, {L"li"}},
    {{L"Lingala"}, {L"ln"}},
    {{L"Lingua Franca Nova"}, {L"lfn"}},
    {{L"Lithuanian"}, {L"lt"}},
    {{L"Lojban"}, {L"jbo"}},
    {{L"Low German"}, {L"nds"}},
    {{L"Macedonian"}, {L"mk"}},
    {{L"Maithili"}, {L"mai"}},
    {{L"Malagasy"}, {L"mg"}},
    {{L"Malay"}, {L"ms"}},
    {{L"Malayalam"}, {L"ml"}},
    {{L"Maltese"}, {L"mt"}},
    {{L"Manx"}, {L"gv"}},
    {{L"Maori"}, {L"mi"}},
    {{L"Marathi"}, {L"mr"}},
    {{L"Mari (Russia)"}, {L"chm"}},
    {{L"Marshallese"}, {L"mh"}},
    {{L"Modern Greek"}, {L"el"}},
    {{L"Mongolian"}, {L"mn"}},
    {{L"Montenegrin"}, {L"cnr"}},
    {{L"Morisyen"}, {L"mfe"}},
    {{L"Navajo"}, {L"nv"}},
    {{L"Nepali"}, {L"ne"}},
    {{L"Niuean"}, {L"niu"}},
    {{L"Northern Sami"}, {L"se"}},
    {{L"Norwegian"}, {L"no"}},
    {{L"Nyanja"}, {L"ny"}},
    {{L"Occitan"}, {L"oc"}},
    {{L"Ojibwa"}, {L"oj"}},
    {{L"Oriya"}, {L"or"}},
    {{L"Oromo"}, {L"om"}},
    {{L"Ossetian"}, {L"os"}},
    {{L"Pam"}, {L"pmn"}},
    {{L"Pangasinan"}, {L"pag"}},
    {{L"Panjabi"}, {L"pa"}},
    {{L"Papiamento"}, {L"pap"}},
    {{L"Persian"}, {L"fa"}},
    {{L"Polish"}, {L"pl"}},
    {{L"Portuguese"}, {L"pt"}},
    {{L"Pushto"}, {L"ps"}},
    {{L"Quechua"}, {L"qu"}},
    {{L"Romanian"}, {L"ro"}},
    {{L"Romansh"}, {L"rm"}},
    {{L"Romany"}, {L"rom"}},
    {{L"Rundi"}, {L"rn"}},
    {{L"Russian"}, {L"ru"}},
    {{L"Rusyn"}, {L"rue"}},
    {{L"Samoan"}, {L"sm"}},
    {{L"Sango"}, {L"sg"}},
    {{L"Sanskrit"}, {L"sa"}},
    {{L"Scots"}, {L"sco"}},
    {{L"Serbo-Croatian"}, {L"hbs"}},
    {{L"Shan"}, {L"shn"}},
    {{L"Shona"}, {L"sn"}},
    {{L"Silesian"}, {L"szl"}},
    {{L"Simplified Chinese"}, {L"zh"}},
    {{L"Sindhi"}, {L"sd"}},
    {{L"Sinhala"}, {L"si"}},
    {{L"Slovak"}, {L"sk"}},
    {{L"Slovenian"}, {L"sl"}},
    {{L"Somali"}, {L"so"}},
    {{L"Spanish"}, {L"es"}},
    {{L"Sundanese"}, {L"su"}},
    {{L"Swahili"}, {L"sw"}},
    {{L"Swedish"}, {L"sv"}},
    {{L"Syriac"}, {L"syr"}},
    {{L"Tagalog"}, {L"tl"}},
    {{L"Tahitian"}, {L"ty"}},
    {{L"Tajik"}, {L"tg"}},
    {{L"Tamil"}, {L"ta"}},
    {{L"Tatar"}, {L"tt"}},
    {{L"Telugu"}, {L"te"}},
    {{L"Tetum"}, {L"tet"}},
    {{L"Thai"}, {L"th"}},
    {{L"Tigrinya"}, {L"ti"}},
    {{L"Tok Pisin"}, {L"tpi"}},
    {{L"Tonga (Tonga Islands)"}, {L"to"}},
    {{L"Tonga (Zambia)"}, {L"toi"}},
    {{L"Traditional Chinese"}, {L"zh-tw"}},
    {{L"Tsonga"}, {L"ts"}},
    {{L"Turkish"}, {L"tr"}},
    {{L"Turkmen"}, {L"tk"}},
    {{L"Tuvalu"}, {L"tvl"}},
    {{L"Tuvinian"}, {L"tyv"}},
    {{L"Twi"}, {L"tw"}},
    {{L"Udmurt"}, {L"udm"}},
    {{L"Umbundu"}, {L"umb"}},
    {{L"Upper Sorbian"}, {L"hsb"}},
    {{L"Urdu"}, {L"ur"}},
    {{L"Uzbek"}, {L"uz"}},
    {{L"Venda"}, {L"ve"}},
    {{L"Vietnamese"}, {L"vi"}},
    {{L"Volapuk"}, {L"vo"}},
    {{L"Walloon"}, {L"wa"}},
    {{L"Waray (Philippines)"}, {L"war"}},
    {{L"Welsh"}, {L"cy"}},
    {{L"Western Frisian"}, {L"fy"}},
    {{L"Wolof"}, {L"wo"}},
    {{L"Xhosa"}, {L"xh"}},
    {{L"Yiddish"}, {L"yi"}},
    {{L"Yoruba"}, {L"yo"}},
    {{L"Zaza"}, {L"zza"}},
    {{L"Zulu"}, {L"zu"}},
    {{L"?"}, {L"auto"}}
};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 50, rateLimitTimespan = 1000, maxSentenceSize = 1000;

// Helper function to generate GMT time string
std::string GetGMTTime()
{
    time_t now = time(nullptr);
    tm gmtTime;
    gmtime_s(&gmtTime, &now);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &gmtTime);
    return buffer;
}

// Helper function to generate unique nonce
std::string GenerateNonce()
{
    static std::atomic<uint64_t> counter = 0;
    return FormatString("%llu-%llu", time(nullptr), counter.fetch_add(1));
}

// Helper function to create Aliyun API signature
std::string CreateAliyunSignature(const std::string& accessKeySecret, const std::string& httpVerb,
    const std::string& accept, const std::string& contentMd5, const std::string& contentType,
    const std::string& date, const std::string& nonce, const std::string& resourcePath)
{
    // Build the string to sign according to Aliyun specification
    std::string stringToSign = httpVerb + "\n" +
        accept + "\n" +
        contentMd5 + "\n" +
        contentType + "\n" +
        date + "\n" +
        "x-acs-signature-method:HMAC-SHA1\n" +
        "x-acs-signature-nonce:" + nonce + "\n" +
        "x-acs-version:2019-01-02\n" +
        resourcePath;

    // Calculate HMAC-SHA1
    std::string hmac = HmacSha1(accessKeySecret, stringToSign);

    // Base64 encode the result
    return Base64Encode(hmac);
}

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
    if (tlp.translateVersion == L"free") {
        // Free version using translate.alibaba.com
        static Synchronized<std::wstring> csrf;
        if (csrf->empty()) {
            if (HttpRequest httpRequest{
                L"Mozilla/5.0 Textractor",
                L"translate.alibaba.com",
                L"GET",
                L"/api/translate/csrftoken"
            }) {
                try {
                    auto parsedJson = json::parse(WideStringToString(httpRequest.response));
                    if (parsedJson.contains("token") && parsedJson["token"].is_string()) {
                        csrf->assign(StringToWideString(parsedJson["token"].get<std::string>()));
                    } else {
                        return {false, L"CSRF token not found in response"};
                    }
                } catch (const json::exception& e) {
                    return {false, FormatString(L"JSON parse error: %s", StringToWideString(e.what()))};
                }
            } else {
                return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
            }
        }

        if (HttpRequest httpRequest{
            L"Mozilla/5.0 Textractor",
            L"translate.alibaba.com",
            L"POST",
            L"/api/translate/text",
            FormatString(R"(srcLang=%S&tgtLang=%S&domain=general&query=%s&_csrf=%s)",
                         codes.at(tlp.translateFrom),
                         codes.at(tlp.translateTo),
                         Escape(WideStringToString(text)),
                         WideStringToString(csrf.Copy())),
            L"Content-Type: application/x-www-form-urlencoded"
        }) {
            try {
                auto parsedJson = json::parse(WideStringToString(httpRequest.response));

                if (parsedJson.contains("data") && parsedJson["data"].is_object() &&
                    parsedJson["data"].contains("translateText") && parsedJson["data"]["translateText"].is_string()) {
                    std::wstring translation = StringToWideString(parsedJson["data"]["translateText"].get<std::string>());
                    return {true, HTML::Unescape(translation)};
                }

                return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
            } catch (const json::exception& e) {
                return {false, FormatString(L"JSON parse error: %s", StringToWideString(e.what()))};
            }
        }
        else return { false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode) };
    }
    else {
        // Parse API key: format is "accessKeyId|accessKeySecret"
        std::wstring authKey = tlp.authKey;
        size_t splitPos = authKey.find(L'|');
        if (splitPos == std::wstring::npos) {
            return {false, L"Invalid API key format. Expected: accessKeyId|accessKeySecret"};
        }

        std::string accessKeyId = WideStringToString(authKey.substr(0, splitPos));
        std::string accessKeySecret = WideStringToString(authKey.substr(splitPos + 1));

        // Determine API endpoint
        std::string apiPath;
        if (tlp.translateVersion == L"pro") {
            apiPath = "/api/translate/web/ecommerce";
        } else {
            apiPath = "/api/translate/web/general";
        }

        json requestJson;
        requestJson["FormatType"] = "text";
        requestJson["SourceLanguage"] = WideStringToString(codes.at(tlp.translateFrom));
        requestJson["TargetLanguage"] = WideStringToString(codes.at(tlp.translateTo));
        requestJson["SourceText"] = WideStringToString(text);
        requestJson["Scene"] = "general";

        std::string requestBody = requestJson.dump();

        std::string md5Raw = Md5Raw(requestBody);
        std::string contentMd5 = Base64Encode(md5Raw);

        // Generate other required fields
        std::string gmtTime = GetGMTTime();
        std::string nonce = GenerateNonce();
        std::string accept = "application/json";
        std::string contentType = "application/json;charset=utf-8";

        // Create signature
        std::string signature = CreateAliyunSignature(
            accessKeySecret,
            "POST",
            accept,
            contentMd5,
            contentType,
            gmtTime,
            nonce,
            apiPath
        );

        // Build authorization header
        std::string authorization = "acs " + accessKeyId + ":" + signature;

        // Build headers
        std::wstring headers = FormatString(
            L"Accept: %S\r\n"
            L"Content-Type: %S\r\n"
            L"Content-MD5: %S\r\n"
            L"Date: %S\r\n"
            L"Authorization: %S\r\n"
            L"x-acs-signature-method: HMAC-SHA1\r\n"
            L"x-acs-signature-nonce: %S\r\n"
            L"x-acs-version: 2019-01-02",
            accept,
            contentType,
            contentMd5,
            gmtTime,
            authorization,
            nonce
        );

        // Make the request
        if (HttpRequest httpRequest{
            L"AlibabaCloud API Workbench",
            L"mt.cn-hangzhou.aliyuncs.com",
            L"POST",
            StringToWideString(apiPath).c_str(),
            requestBody,
            headers.c_str(),
            INTERNET_DEFAULT_HTTP_PORT,
            nullptr,
            0
        }) {
            try {
                auto parsedJson = json::parse(WideStringToString(httpRequest.response));

                // Check for error
                if (parsedJson.contains("errorCode")) {
                    std::string errorCode = parsedJson["errorCode"].get<std::string>();
                    std::string errorMsg;
                    if (parsedJson.contains("errorMsg") && parsedJson["errorMsg"].is_string()) {
                        errorMsg = parsedJson["errorMsg"].get<std::string>();
                        return {false, FormatString(L"Aliyun API Error [%s]: %s",
                            StringToWideString(errorCode), StringToWideString(errorMsg))};
                    }
                    return {false, FormatString(L"Aliyun API Error: %s", StringToWideString(errorCode))};
                }

                // Extract translation from response
                if (parsedJson.contains("Data") && parsedJson["Data"].is_object() &&
                    parsedJson["Data"].contains("Translated") && parsedJson["Data"]["Translated"].is_string()) {
                    std::wstring translation = StringToWideString(parsedJson["Data"]["Translated"].get<std::string>());
                    return {true, translation};
                }

                return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
            } catch (const json::exception& e) {
                return {false, FormatString(L"JSON parse error: %s", StringToWideString(e.what()))};
            }
        }
        else {
            return { false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode) };
        }
    }
}
