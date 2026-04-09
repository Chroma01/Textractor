#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern const wchar_t *TRANSLATION_ERROR;

const char *TRANSLATION_PROVIDER = "Lingocloud Translate";
const char *GET_API_KEY_FROM = "https://docs.caiyunapp.com/lingocloud-api/index.html";
extern const QStringList languagesFrom
{
	"Chinese (Simplified)",
	"Chinese (Traditional)",
	"English",
	"Japanese",
	"Korean",
	"German",
	"Spanish",
	"French",
	"Italian",
	"Portuguese",
	"Russian",
	"Turkish",
	"Vietnamese"
};

// 你这个目标语言支持也太寒酸了
extern const QStringList languagesTo
{
	"Chinese (Simplified)",
	"Chinese (Traditional)",
	"English",
	"Japanese",
	"Korean"
};

extern const std::unordered_map<std::wstring, std::wstring> codes
{
	    {{L"Chinese (Simplified)"},   {L"zh"}},
		{{L"Chinese (Traditional)"},  {L"zh-Hant"}},
		{{L"English"},               {L"en"}},
		{{L"Japanese"},              {L"ja"}},
		{{L"Korean"},                {L"ko"}},
		{{L"German"},                {L"de"}},
		{{L"Spanish"},               {L"es"}},
		{{L"French"},                {L"fr"}},
		{{L"Italian"},               {L"it"}},
		{{L"Portuguese"},            {L"pt"}},
		{{L"Russian"},               {L"ru"}},
		{{L"Turkish"},               {L"tr"}},
		{{L"Vietnamese"},            {L"vi"}},
		{{L"?"},                     {L"auto"}}
};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 10, rateLimitTimespan = 1000, maxSentenceSize = 1000;

namespace
{
	constexpr auto LINGOCLOUD_AUTH_PROMPT = L"Please fill in the API token for Lingocloud Translation in the API key field";

	std::wstring BuildTransType(const std::wstring& from, const std::wstring& to)
	{
		// Build trans_type like "en2zh", "ja2zh", "auto2zh", etc.
		return from + L"2" + to;
	}

}

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
	try {
		if (tlp.authKey.empty()) return { false, LINGOCLOUD_AUTH_PROMPT };

		std::wstring token = tlp.authKey;
		Trim(token);
		if (token.empty()) return { false, LINGOCLOUD_AUTH_PROMPT };

		std::wstring from = codes.at(tlp.translateFrom);
		std::wstring to = codes.at(tlp.translateTo);
		std::wstring transType = BuildTransType(from, to);

		// Build JSON request body
		json requestBody;
		requestBody["source"] = json::array({WideStringToString(text)});
		requestBody["trans_type"] = WideStringToString(transType);
		requestBody["detect"] = (from == L"auto");

		std::string body = requestBody.dump();

		// Build authorization header
		std::wstring authHeader = L"Content-Type: application/json\r\nx-authorization: token " + token;

		if (HttpRequest httpRequest{
			L"Mozilla/5.0 Textractor",
			L"api.interpreter.caiyunai.com",
			L"POST",
			L"/v1/translator",
			body,
			authHeader.c_str()
		}) {
			try {
				auto parsedJson = json::parse(WideStringToString(httpRequest.response));

				// Check for error
				if (parsedJson.contains("error"))
				{
					std::string errorMessage = parsedJson["error"];
					return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, StringToWideString(errorMessage)) };
				}

				if (parsedJson.contains("rc") && parsedJson["rc"].is_number())
				{
					int rc = parsedJson["rc"];
					if (rc != 0)
					{
						std::string errorMsg = parsedJson.contains("message") ? parsedJson["message"].get<std::string>() : "Unknown error";
						return { false, FormatString(L"%s (rc=%d): %s", TRANSLATION_ERROR, rc, StringToWideString(errorMsg)) };
					}
				}

				// Get translation result
				if (!parsedJson.contains("target") || !parsedJson["target"].is_array() || parsedJson["target"].empty())
					return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response) };

				std::wstring combinedTranslation;
				for (const auto& translation : parsedJson["target"])
				{
					if (translation.is_string())
					{
						if (!combinedTranslation.empty()) combinedTranslation += L"\n";
						combinedTranslation += StringToWideString(translation.get<std::string>());
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
