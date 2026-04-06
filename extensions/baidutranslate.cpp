#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"
#include "hashutils.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern const wchar_t *TRANSLATION_ERROR;

// 用百度翻译翻H文会不会封号?

const char *TRANSLATION_PROVIDER = "Baidu Translate";
const char *GET_API_KEY_FROM = "https://fanyi-api.baidu.com/manage/developer";
extern const QStringList languagesTo
{
    "Chinese (Simplified)",
    "English",
    "Cantonese",
    "Classical Chinese",
    "Japanese",
    "Korean",
    "French",
    "Spanish",
    "Thai",
    "Arabic",
    "Russian",
    "Portuguese",
    "German",
    "Italian",
    "Greek",
    "Dutch",
    "Polish",
    "Bulgarian",
    "Estonian",
    "Danish",
    "Finnish",
    "Czech",
    "Romanian",
    "Slovenian",
    "Swedish",
    "Hungarian",
    "Chinese (Traditional)",
    "Vietnamese"
}, languagesFrom = languagesTo;
extern const std::unordered_map<std::wstring, std::wstring> codes
{
        {{L"Chinese (Simplified)"}, {L"zh"}},
        {{L"English"}, {L"en"}},
        {{L"Cantonese"}, {L"yue"}},
        {{L"Classical Chinese"}, {L"wyw"}},
        {{L"Japanese"}, {L"jp"}},
        {{L"Korean"}, {L"kor"}},
        {{L"French"}, {L"fra"}},
        {{L"Spanish"}, {L"spa"}},
        {{L"Thai"}, {L"th"}},
        {{L"Arabic"}, {L"ara"}},
        {{L"Russian"}, {L"ru"}},
        {{L"Portuguese"}, {L"pt"}},
        {{L"German"}, {L"de"}},
        {{L"Italian"}, {L"it"}},
        {{L"Greek"}, {L"el"}},
        {{L"Dutch"}, {L"nl"}},
        {{L"Polish"}, {L"pl"}},
        {{L"Bulgarian"}, {L"bul"}},
        {{L"Estonian"}, {L"est"}},
        {{L"Danish"}, {L"dan"}},
        {{L"Finnish"}, {L"fin"}},
        {{L"Czech"}, {L"cs"}},
        {{L"Romanian"}, {L"rom"}},
        {{L"Slovenian"}, {L"slo"}},
        {{L"Swedish"}, {L"swe"}},
        {{L"Hungarian"}, {L"hu"}},
        {{L"Chinese (Traditional)"}, {L"cht"}},
        {{L"Vietnamese"}, {L"vie"}},
        {{L"?"}, {L"auto"}}
};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 2000;

namespace
{
	constexpr auto BAIDU_AUTH_PROMPT = L"Please fill in the appid|appkey for Baidu Translation in the API key field";

	struct BaiduCredentials
	{
		std::wstring appId;
		std::wstring appKey;
	};

	std::optional<BaiduCredentials> ParseCredentials(std::wstring authKey)
	{
		Trim(authKey);
		if (authKey.empty()) return {};

		size_t separator = authKey.find_first_of(L"|,:;");
		if (separator == std::wstring::npos) separator = authKey.find_first_of(L" \t");
		if (separator == std::wstring::npos) return {};

		BaiduCredentials credentials{
			authKey.substr(0, separator),
			authKey.substr(separator + 1)
		};
		Trim(credentials.appId);
		Trim(credentials.appKey);
		if (credentials.appId.empty() || credentials.appKey.empty()) return {};
		return credentials;
	}

}

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
	try {
		if (tlp.authKey.empty()) return { false, BAIDU_AUTH_PROMPT };
		auto credentials = ParseCredentials(tlp.authKey);
		if (!credentials) return { false, BAIDU_AUTH_PROMPT };

		std::wstring from = codes.at(tlp.translateFrom), to = codes.at(tlp.translateTo);
		std::string query = WideStringToString(text);
		std::string appId = WideStringToString(credentials->appId);
		std::string appKey = WideStringToString(credentials->appKey);
		std::string salt = FormatString("%llu%lu", GetTickCount64(), GetCurrentThreadId());
		std::string sign = Md5(appId + query + salt + appKey);
		if (sign.empty()) return { false, L"MD5 signature generation failed." };

		std::string body = FormatString(
			"q=%s&from=%s&to=%s&appid=%s&salt=%s&sign=%s",
			Escape(query),
			WideStringToString(from),
			WideStringToString(to),
			appId,
			salt,
			sign
		);

		if (HttpRequest httpRequest{
			L"Mozilla/5.0 Textractor",
			L"fanyi-api.baidu.com",
			L"POST",
			L"/api/trans/vip/translate",
			body,
			L"Content-Type: application/x-www-form-urlencoded"
		}) {
			try {
				auto parsedJson = json::parse(WideStringToString(httpRequest.response));

				if (parsedJson.contains("error_code"))
				{
					std::string errorCode = parsedJson["error_code"];
					std::string errorMessage;
					if (parsedJson.contains("error_msg"))
						errorMessage = parsedJson["error_msg"];
					if (!errorMessage.empty())
						return { false, FormatString(L"%s (%s): %s", TRANSLATION_ERROR, StringToWideString(errorCode), StringToWideString(errorMessage)) };
					return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, StringToWideString(errorCode)) };
				}

				if (!parsedJson.contains("trans_result") || !parsedJson["trans_result"].is_array() || parsedJson["trans_result"].empty())
					return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response) };

				std::wstring combinedTranslation;
				for (const auto& translation : parsedJson["trans_result"])
				{
					if (translation.contains("dst") && translation["dst"].is_string())
					{
						if (!combinedTranslation.empty()) combinedTranslation += L"\n";
						combinedTranslation += StringToWideString(translation["dst"].get<std::string>());
					}
				}

				if (!combinedTranslation.empty()) return { true, combinedTranslation };
				return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response) };
			}
			catch (const json::exception& e)
			{
				return { false, FormatString(L"%s: JSON parse error: %s", TRANSLATION_ERROR, StringToWideString(e.what()).c_str()) };
			}
		} else return { false, FormatString(L"%s (code=%lu)", TRANSLATION_ERROR, httpRequest.errorCode) };
	} catch (const std::out_of_range &e) {
		return {false, FormatString(L"Key error in translation map: %s", StringToWideString(e.what()).c_str())};
	}
	catch (const std::exception &e) {
		return {false, FormatString(L"Exception occurred: %s", StringToWideString(e.what()).c_str())};
	}
	catch (...) {
		return {false, L"Unknown error occurred during translation"};
	}
}
