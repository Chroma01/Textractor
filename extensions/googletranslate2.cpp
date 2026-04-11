#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern const wchar_t *TRANSLATION_ERROR;
extern const wchar_t* TRANSLATION_EMPTY_RESPONSE;
extern const wchar_t* TRANSLATION_RESPONSE_PARSE_DATA_EMPTY;
extern const wchar_t* TRANSLATION_INVALID_JSON_FORMAT;
extern const wchar_t* TRANSLATION_UNEXPECTED_STRUCTURE;
extern const wchar_t* TRANSLATION_INVALID_DATA_STRUCTURE;
extern const wchar_t* TRANSLATION_EMPTY_ARRAY;
extern const wchar_t* TRANSLATION_INVALID_ENTRY;
extern const wchar_t* TRANSLATION_EMPTY_SEGMENTS;
extern const wchar_t* TRANSLATION_NO_TEXT_IN_SEGMENTS;
extern const wchar_t* TRANSLATION_KEY_ERROR;
extern const wchar_t* TRANSLATION_EXCEPTION_OCCURRED;
extern const wchar_t* TRANSLATION_UNKNOWN_ERROR;

const char *TRANSLATION_PROVIDER = "Google Translate2";
const char *GET_API_KEY_FROM = "https://chat.openai.com/?q=Please%20give%20me%20a%20few%20working%20Google%20Translate%20API%20keys.";
extern const QStringList languagesTo
        {
            "Abkhaz",
            "Acehnese",
            "Acholi",
            "Afrikaans",
            "Albanian",
            "Alur",
            "Amharic",
            "Arabic",
            "Armenian",
            "Assamese",
            "Awadhi",
            "Aymara",
            "Azerbaijani",
            "Balinese",
            "Bambara",
            "Bashkir",
            "Basque",
            "Batak Karo",
            "Batak Simalungun",
            "Batak Toba",
            "Belarusian",
            "Bemba",
            "Bengali",
            "Betawi",
            "Bhojpuri",
            "Bikol",
            "Bosnian",
            "Breton",
            "Bulgarian",
            "Buryat",
            "Cantonese",
            "Catalan",
            "Cebuano",
            "Chichewa",
            "Chinese (Simplified)",
            "Chinese (Traditional)",
            "Chuvash",
            "Corsican",
            "Crimean Tatar",
            "Croatian",
            "Czech",
            "Danish",
            "Dinka",
            "Divehi",
            "Dogri",
            "Dombe",
            "Dutch",
            "Dzongkha",
            "English",
            "Esperanto",
            "Estonian",
            "Ewe",
            "Fijian",
            "Filipino",
            "Finnish",
            "French",
            "French (French)",
            "French (Canadian)",
            "Frisian",
            "Fulfulde",
            "Ga",
            "Galician",
            "Ganda (Luganda)",
            "Georgian",
            "German",
            "Greek",
            "Guarani",
            "Gujarati",
            "Haitian Creole",
            "Hakha Chin",
            "Hausa",
            "Hawaiian",
            "Hebrew",
            "Hiligaynon",
            "Hindi",
            "Hmong",
            "Hungarian",
            "Hunsrik",
            "Icelandic",
            "Igbo",
            "Iloko",
            "Indonesian",
            "Irish",
            "Italian",
            "Japanese",
            "Javanese",
            "Kannada",
            "Kapampangan",
            "Kazakh",
            "Khmer",
            "Kiga",
            "Kinyarwanda",
            "Kituba",
            "Konkani",
            "Korean",
            "Krio",
            "Kurdish (Kurmanji)",
            "Kurdish (Sorani)",
            "Kyrgyz",
            "Lao",
            "Latgalian",
            "Latin",
            "Latvian",
            "Ligurian",
            "Limburgan",
            "Lingala",
            "Lithuanian",
            "Lombard",
            "Luo",
            "Luxembourgish",
            "Macedonian",
            "Maithili",
            "Makassar",
            "Malagasy",
            "Malay",
            "Malay (Jawi)",
            "Malayalam",
            "Maltese",
            "Maori",
            "Marathi",
            "Meadow Mari",
            "Meiteilon (Manipuri)",
            "Minang",
            "Mizo",
            "Mongolian",
            "Myanmar (Burmese)",
            "Ndebele (South)",
            "Nepalbhasa (Newari)",
            "Nepali",
            "Northern Sotho (Sepedi)",
            "Norwegian",
            "Nuer",
            "Occitan",
            "Odia (Oriya)",
            "Oromo",
            "Pangasinan",
            "Papiamento",
            "Pashto",
            "Persian",
            "Polish",
            "Portuguese",
            "Portuguese (Portugal)",
            "Portuguese (Brazil)",
            "Punjabi",
            "Punjabi (Shahmukhi)",
            "Quechua",
            "Romani",
            "Romanian",
            "Rundi",
            "Russian",
            "Samoan",
            "Sango",
            "Sanskrit",
            "Scots Gaelic",
            "Serbian",
            "Sesotho",
            "Seychellois Creole",
            "Shan",
            "Shona",
            "Sicilian",
            "Silesian",
            "Sindhi",
            "Sinhala",
            "Slovak",
            "Slovenian",
            "Somali",
            "Spanish",
            "Sundanese",
            "Swahili",
            "Swati",
            "Swedish",
            "Tajik",
            "Tamil",
            "Tatar",
            "Telugu",
            "Tetum",
            "Thai",
            "Tigrinya",
            "Tsonga",
            "Tswana",
            "Turkish",
            "Turkmen",
            "Twi (Akan)",
            "Ukrainian",
            "Urdu",
            "Uyghur",
            "Uzbek",
            "Vietnamese",
            "Welsh",
            "Xhosa",
            "Yiddish",
            "Yoruba",
            "Yucatec Maya",
            "Zulu",
        }, languagesFrom = languagesTo;
extern const std::unordered_map<std::wstring, std::wstring> codes
{
    {{L"Abkhaz"}, {L"ab"}},
    {{L"Acehnese"}, {L"ace"}},
    {{L"Acholi"}, {L"ach"}},
    {{L"Afrikaans"}, {L"af"}},
    {{L"Albanian"}, {L"sq"}},
    {{L"Alur"}, {L"alz"}},
    {{L"Amharic"}, {L"am"}},
    {{L"Arabic"}, {L"ar"}},
    {{L"Armenian"}, {L"hy"}},
    {{L"Assamese"}, {L"as"}},
    {{L"Awadhi"}, {L"awa"}},
    {{L"Aymara"}, {L"ay"}},
    {{L"Azerbaijani"}, {L"az"}},
    {{L"Balinese"}, {L"ban"}},
    {{L"Bambara"}, {L"bm"}},
    {{L"Bashkir"}, {L"ba"}},
    {{L"Basque"}, {L"eu"}},
    {{L"Batak Karo"}, {L"btx"}},
    {{L"Batak Simalungun"}, {L"bts"}},
    {{L"Batak Toba"}, {L"bbc"}},
    {{L"Belarusian"}, {L"be"}},
    {{L"Bemba"}, {L"bem"}},
    {{L"Bengali"}, {L"bn"}},
    {{L"Betawi"}, {L"bew"}},
    {{L"Bhojpuri"}, {L"bho"}},
    {{L"Bikol"}, {L"bik"}},
    {{L"Bosnian"}, {L"bs"}},
    {{L"Breton"}, {L"br"}},
    {{L"Bulgarian"}, {L"bg"}},
    {{L"Buryat"}, {L"bua"}},
    {{L"Cantonese"}, {L"yue"}},
    {{L"Catalan"}, {L"ca"}},
    {{L"Cebuano"}, {L"ceb"}},
    {{L"Chichewa"}, {L"ny"}},
    {{L"Chinese (Simplified)"}, {L"zh-CN"}},
    {{L"Chinese (Traditional)"}, {L"zh-TW"}},
    {{L"Chuvash"}, {L"cv"}},
    {{L"Corsican"}, {L"co"}},
    {{L"Crimean Tatar"}, {L"crh"}},
    {{L"Croatian"}, {L"hr"}},
    {{L"Czech"}, {L"cs"}},
    {{L"Danish"}, {L"da"}},
    {{L"Dinka"}, {L"din"}},
    {{L"Divehi"}, {L"dv"}},
    {{L"Dogri"}, {L"doi"}},
    {{L"Dombe"}, {L"dov"}},
    {{L"Dutch"}, {L"nl"}},
    {{L"Dzongkha"}, {L"dz"}},
    {{L"English"}, {L"en"}},
    {{L"Esperanto"}, {L"eo"}},
    {{L"Estonian"}, {L"et"}},
    {{L"Ewe"}, {L"ee"}},
    {{L"Fijian"}, {L"fj"}},
    {{L"Filipino"}, {L"tl"}},
    {{L"Finnish"}, {L"fi"}},
    {{L"French"}, {L"fr"}},
    {{L"French (French)"}, {L"fr-FR"}},
    {{L"French (Canadian)"}, {L"fr-CA"}},
    {{L"Frisian"}, {L"fy"}},
    {{L"Fulfulde"}, {L"ff"}},
    {{L"Ga"}, {L"gaa"}},
    {{L"Galician"}, {L"gl"}},
    {{L"Ganda (Luganda)"}, {L"lg"}},
    {{L"Georgian"}, {L"ka"}},
    {{L"German"}, {L"de"}},
    {{L"Greek"}, {L"el"}},
    {{L"Guarani"}, {L"gn"}},
    {{L"Gujarati"}, {L"gu"}},
    {{L"Haitian Creole"}, {L"ht"}},
    {{L"Hakha Chin"}, {L"cnh"}},
    {{L"Hausa"}, {L"ha"}},
    {{L"Hawaiian"}, {L"haw"}},
    {{L"Hebrew"}, {L"iw"}},
    {{L"Hiligaynon"}, {L"hil"}},
    {{L"Hindi"}, {L"hi"}},
    {{L"Hmong"}, {L"hmn"}},
    {{L"Hungarian"}, {L"hu"}},
    {{L"Hunsrik"}, {L"hrx"}},
    {{L"Icelandic"}, {L"is"}},
    {{L"Igbo"}, {L"ig"}},
    {{L"Iloko"}, {L"ilo"}},
    {{L"Indonesian"}, {L"id"}},
    {{L"Irish"}, {L"ga"}},
    {{L"Italian"}, {L"it"}},
    {{L"Japanese"}, {L"ja"}},
    {{L"Javanese"}, {L"jw"}},
    {{L"Kannada"}, {L"kn"}},
    {{L"Kapampangan"}, {L"pam"}},
    {{L"Kazakh"}, {L"kk"}},
    {{L"Khmer"}, {L"km"}},
    {{L"Kiga"}, {L"cgg"}},
    {{L"Kinyarwanda"}, {L"rw"}},
    {{L"Kituba"}, {L"ktu"}},
    {{L"Konkani"}, {L"gom"}},
    {{L"Korean"}, {L"ko"}},
    {{L"Krio"}, {L"kri"}},
    {{L"Kurdish (Kurmanji)"}, {L"ku"}},
    {{L"Kurdish (Sorani)"}, {L"ckb"}},
    {{L"Kyrgyz"}, {L"ky"}},
    {{L"Lao"}, {L"lo"}},
    {{L"Latgalian"}, {L"ltg"}},
    {{L"Latin"}, {L"la"}},
    {{L"Latvian"}, {L"lv"}},
    {{L"Ligurian"}, {L"lij"}},
    {{L"Limburgan"}, {L"li"}},
    {{L"Lingala"}, {L"ln"}},
    {{L"Lithuanian"}, {L"lt"}},
    {{L"Lombard"}, {L"lmo"}},
    {{L"Luo"}, {L"luo"}},
    {{L"Luxembourgish"}, {L"lb"}},
    {{L"Macedonian"}, {L"mk"}},
    {{L"Maithili"}, {L"mai"}},
    {{L"Makassar"}, {L"mak"}},
    {{L"Malagasy"}, {L"mg"}},
    {{L"Malay"}, {L"ms"}},
    {{L"Malay (Jawi)"}, {L"ms-Arab"}},
    {{L"Malayalam"}, {L"ml"}},
    {{L"Maltese"}, {L"mt"}},
    {{L"Maori"}, {L"mi"}},
    {{L"Marathi"}, {L"mr"}},
    {{L"Meadow Mari"}, {L"chm"}},
    {{L"Meiteilon (Manipuri)"}, {L"mni-Mtei"}},
    {{L"Minang"}, {L"min"}},
    {{L"Mizo"}, {L"lus"}},
    {{L"Mongolian"}, {L"mn"}},
    {{L"Myanmar (Burmese)"}, {L"my"}},
    {{L"Ndebele (South)"}, {L"nr"}},
    {{L"Nepalbhasa (Newari)"}, {L"new"}},
    {{L"Nepali"}, {L"ne"}},
    {{L"Northern Sotho (Sepedi)"}, {L"nso"}},
    {{L"Norwegian"}, {L"no"}},
    {{L"Nuer"}, {L"nus"}},
    {{L"Occitan"}, {L"oc"}},
    {{L"Odia (Oriya)"}, {L"or"}},
    {{L"Oromo"}, {L"om"}},
    {{L"Pangasinan"}, {L"pag"}},
    {{L"Papiamento"}, {L"pap"}},
    {{L"Pashto"}, {L"ps"}},
    {{L"Persian"}, {L"fa"}},
    {{L"Polish"}, {L"pl"}},
    {{L"Portuguese"}, {L"pt"}},
    {{L"Portuguese (Portugal)"}, {L"pt-PT"}},
    {{L"Portuguese (Brazil)"}, {L"pt-BR"}},
    {{L"Punjabi"}, {L"pa"}},
    {{L"Punjabi (Shahmukhi)"}, {L"pa-Arab"}},
    {{L"Quechua"}, {L"qu"}},
    {{L"Romani"}, {L"rom"}},
    {{L"Romanian"}, {L"ro"}},
    {{L"Rundi"}, {L"rn"}},
    {{L"Russian"}, {L"ru"}},
    {{L"Samoan"}, {L"sm"}},
    {{L"Sango"}, {L"sg"}},
    {{L"Sanskrit"}, {L"sa"}},
    {{L"Scots Gaelic"}, {L"gd"}},
    {{L"Serbian"}, {L"sr"}},
    {{L"Sesotho"}, {L"st"}},
    {{L"Seychellois Creole"}, {L"crs"}},
    {{L"Shan"}, {L"shn"}},
    {{L"Shona"}, {L"sn"}},
    {{L"Sicilian"}, {L"scn"}},
    {{L"Silesian"}, {L"szl"}},
    {{L"Sindhi"}, {L"sd"}},
    {{L"Sinhala"}, {L"si"}},
    {{L"Slovak"}, {L"sk"}},
    {{L"Slovenian"}, {L"sl"}},
    {{L"Somali"}, {L"so"}},
    {{L"Spanish"}, {L"es"}},
    {{L"Sundanese"}, {L"su"}},
    {{L"Swahili"}, {L"sw"}},
    {{L"Swati"}, {L"ss"}},
    {{L"Swedish"}, {L"sv"}},
    {{L"Tajik"}, {L"tg"}},
    {{L"Tamil"}, {L"ta"}},
    {{L"Tatar"}, {L"tt"}},
    {{L"Telugu"}, {L"te"}},
    {{L"Tetum"}, {L"tet"}},
    {{L"Thai"}, {L"th"}},
    {{L"Tigrinya"}, {L"ti"}},
    {{L"Tsonga"}, {L"ts"}},
    {{L"Tswana"}, {L"tn"}},
    {{L"Turkish"}, {L"tr"}},
    {{L"Turkmen"}, {L"tk"}},
    {{L"Twi (Akan)"}, {L"ak"}},
    {{L"Ukrainian"}, {L"uk"}},
    {{L"Urdu"}, {L"ur"}},
    {{L"Uyghur"}, {L"ug"}},
    {{L"Uzbek"}, {L"uz"}},
    {{L"Vietnamese"}, {L"vi"}},
    {{L"Welsh"}, {L"cy"}},
    {{L"Xhosa"}, {L"xh"}},
    {{L"Yiddish"}, {L"yi"}},
    {{L"Yoruba"}, {L"yo"}},
    {{L"Yucatec Maya"}, {L"yua"}},
    {{L"Zulu"}, {L"zu"}},
    {{L"?"}, {L"auto"}}
};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 1000;

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
    try {
        std::wstring from = codes.at(tlp.translateFrom), to = codes.at(tlp.translateTo);

        // // authKey 为空: 使用 AI 增强翻译 (参数为 1)
        // // authKey 不为空: 使用普通翻译 (参数为 2)
        // int translationMode = tlp.authKey.empty() ? 1 : 2;
        int translationMode = 2; // 经过测试，1不起作用😂

        // f.req=[[["MkEWBc","[[\"文本\",\"源语言\",\"目标语言\",1,null,模式],[]]",null,"generic"]]]
        json innerArray = json::array({
            WideStringToString(text),
            WideStringToString(from),
            WideStringToString(to),
            1,
            nullptr,
            translationMode
        });

        json requestData = json::array({
            json::array({
                json::array({
                    "MkEWBc",
                    json::array({innerArray, json::array()}).dump(),
                    nullptr,
                    "generic"
                })
            })
        });

        std::string requestBody = "f.req=" + Escape(requestData.dump()) + "&";

        if (HttpRequest httpRequest{
            L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/143.0.0.0 Safari/537.36 Textractor",
            L"translate.google.com",
            L"POST",
            L"/_/TranslateWebserverUi/data/batchexecute?rpcids=MkEWBc",
            requestBody,
            L"Content-Type: application/x-www-form-urlencoded;charset=UTF-8",
            INTERNET_DEFAULT_HTTPS_PORT
        }) {
            if (httpRequest.response.empty()) {
                return {false, TRANSLATION_EMPTY_RESPONSE};
            }

            try {
                std::wstring responseStr = httpRequest.response;

                size_t jsonStart = responseStr.find(L")]}'");
                if (jsonStart != std::wstring::npos) {
                    responseStr = responseStr.substr(jsonStart + 4);
                }
                size_t firstNonWhite = responseStr.find_first_not_of(L" \t\r\n");
                if (firstNonWhite != std::wstring::npos) {
                    responseStr = responseStr.substr(firstNonWhite);
                }
                std::wstring dataLine;
                size_t firstNewline = responseStr.find(L'\n');
                if (firstNewline != std::wstring::npos) {
                    dataLine = responseStr.substr(firstNewline + 1);
                    size_t secondNewline = dataLine.find(L'\n');
                    if (secondNewline != std::wstring::npos) {
                        dataLine = dataLine.substr(0, secondNewline);
                    }
                } else {
                    dataLine = responseStr;
                }
                size_t dataStart = dataLine.find_first_not_of(L" \t\r\n");
                if (dataStart != std::wstring::npos) {
                    dataLine = dataLine.substr(dataStart);
                }
                size_t dataEnd = dataLine.find_last_not_of(L" \t\r\n");
                if (dataEnd != std::wstring::npos) {
                    dataLine = dataLine.substr(0, dataEnd + 1);
                }
                if (dataLine.empty()) {
                    return {false, TRANSLATION_RESPONSE_PARSE_DATA_EMPTY};
                }

                auto responseJson = json::parse(WideStringToString(dataLine));
                if (!responseJson.is_array() || responseJson.empty()) {
                    return {false, TRANSLATION_INVALID_JSON_FORMAT};
                }
                auto firstElement = responseJson[0];
                if (!firstElement.is_array() || firstElement.size() < 3) {
                    return {false, TRANSLATION_UNEXPECTED_STRUCTURE};
                }
                std::string translationDataStr = firstElement[2];
                auto translationData = json::parse(translationDataStr);
                if (!translationData.is_array() || translationData.size() < 2) {
                    return {false, TRANSLATION_INVALID_DATA_STRUCTURE};
                }
                auto translationArray = translationData[1];
                if (!translationArray.is_array() || translationArray.empty()) {
                    return {false, TRANSLATION_EMPTY_ARRAY};
                }
                auto firstTranslation = translationArray[0][0];
                if (!firstTranslation.is_array() || firstTranslation.size() < 6) {
                    return {false, TRANSLATION_INVALID_ENTRY};
                }
                auto translationSegments = firstTranslation[5];
                if (!translationSegments.is_array() || translationSegments.empty()) {
                    return {false, TRANSLATION_EMPTY_SEGMENTS};
                }

                // 拼接所有翻译片段
                std::wstring fullTranslation;
                for (const auto& segment : translationSegments) {
                    if (segment.is_array() && !segment.empty() && segment[0].is_string()) {
                        std::string segmentText = segment[0];
                        fullTranslation += StringToWideString(segmentText);
                    }
                }

                if (fullTranslation.empty()) {
                    return {false, TRANSLATION_NO_TEXT_IN_SEGMENTS};
                }

                return {true, fullTranslation};

            } catch (const json::exception& e) {
                return {false, FormatString(L"%s: JSON parse error: %s - Response: %s", TRANSLATION_ERROR,
                                          StringToWideString(e.what()).c_str(), httpRequest.response.c_str())};
            }
        } else {
            return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
        }
    } catch (const std::out_of_range &e) {
        return {false, FormatString(L"%s: %s", TRANSLATION_KEY_ERROR, StringToWideString(e.what()).c_str())};
    }
    catch (const std::exception &e) {
        return {false, FormatString(L"%s: %s", TRANSLATION_EXCEPTION_OCCURRED, StringToWideString(e.what()).c_str())};
    }
    catch (...) {
        return {false, TRANSLATION_UNKNOWN_ERROR};
    }
}
