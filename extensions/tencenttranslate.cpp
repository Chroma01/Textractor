#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"
#include "hashutils.h"
#include <nlohmann/json.hpp>
#include <ctime>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

extern const wchar_t *TRANSLATION_ERROR;

const char *TRANSLATION_PROVIDER = "Tencent Translate";
const char *GET_API_KEY_FROM = "https://cloud.tencent.com/document/product/551";
extern const QStringList languagesTo
{
	"Chinese (Simplified)",
	"Chinese (Traditional)",
	"English",
	"Japanese",
	"Korean",
	"French",
	"Spanish",
	"Italian",
	"German",
	"Turkish",
	"Russian",
	"Portuguese",
	"Vietnamese",
	"Indonesian",
	"Thai",
	"Malay",
	"Arabic",
	"Hindi"
}, languagesFrom = languagesTo;

extern const std::unordered_map<std::wstring, std::wstring> codes
{
	    {{L"Chinese (Simplified)"}, {L"zh"}},
		{{L"Chinese (Traditional)"}, {L"zh-TW"}},
		{{L"English"}, {L"en"}},
		{{L"Japanese"}, {L"ja"}},
		{{L"Korean"}, {L"ko"}},
		{{L"French"}, {L"fr"}},
		{{L"Spanish"}, {L"es"}},
		{{L"Italian"}, {L"it"}},
		{{L"German"}, {L"de"}},
		{{L"Turkish"}, {L"tr"}},
		{{L"Russian"}, {L"ru"}},
		{{L"Portuguese"}, {L"pt"}},
		{{L"Vietnamese"}, {L"vi"}},
		{{L"Indonesian"}, {L"id"}},
		{{L"Thai"}, {L"th"}},
		{{L"Malay"}, {L"ms"}},
		{{L"Arabic"}, {L"ar"}},
		{{L"Hindi"}, {L"hi"}},
};

bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 5, rateLimitTimespan = 1000, maxSentenceSize = 2000;

namespace
{
	constexpr auto TENCENT_AUTH_PROMPT = L"Please fill in the SecretId|SecretKey for Tencent Cloud Translation in the API key field";

	struct TencentCredentials
	{
		std::wstring secretId;
		std::wstring secretKey;
	};

	std::optional<TencentCredentials> ParseCredentials(std::wstring authKey)
	{
		Trim(authKey);
		if (authKey.empty()) return {};

		size_t separator = authKey.find_first_of(L"|,:;");
		if (separator == std::wstring::npos) separator = authKey.find_first_of(L" \t");
		if (separator == std::wstring::npos) return {};

		TencentCredentials credentials{
			authKey.substr(0, separator),
			authKey.substr(separator + 1)
		};
		Trim(credentials.secretId);
		Trim(credentials.secretKey);
		if (credentials.secretId.empty() || credentials.secretKey.empty()) return {};
		return credentials;
	}

	// Convert bytes to hex string (lowercase)
	std::string BytesToHex(const std::string& bytes)
	{
		std::stringstream ss;
		for (unsigned char c : bytes)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
		}
		return ss.str();
	}

	// TC3-HMAC-SHA256 signature algorithm
	std::string TC3Sign(const std::string& secretKey, const std::string& date, const std::string& service, const std::string& stringToSign)
	{
		std::string kDate = HmacSha256("TC3" + secretKey, date);
		std::string kService = HmacSha256(kDate, service);
		std::string kSigning = HmacSha256(kService, "tc3_request");
		std::string signature = HmacSha256(kSigning, stringToSign);
		return BytesToHex(signature);
	}
}

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
	try {
		if (tlp.authKey.empty()) return { false, TENCENT_AUTH_PROMPT };
		auto credentials = ParseCredentials(tlp.authKey);
		if (!credentials) return { false, TENCENT_AUTH_PROMPT };

		// Tencent Cloud API parameters
		std::string service = "tmt";
		std::string host = "tmt.tencentcloudapi.com";
		std::string region = "ap-shanghai";
		std::string action = "TextTranslate";
		std::string version = "2018-03-21";
		std::string algorithm = "TC3-HMAC-SHA256";

		// Get current timestamp
		time_t timestamp = time(nullptr);
		char dateBuffer[11];
		strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", gmtime(&timestamp));
		std::string date(dateBuffer);

		// Prepare request body
		std::wstring from = codes.at(tlp.translateFrom), to = codes.at(tlp.translateTo);

		json payloadJson;
		payloadJson["SourceText"] = WideStringToString(text);
		payloadJson["Source"] = WideStringToString(from);
		payloadJson["Target"] = WideStringToString(to);
		payloadJson["ProjectId"] = 0;

		std::string payload = payloadJson.dump();

		// Step 1: Create canonical request
		std::string httpRequestMethod = "POST";
		std::string canonicalUri = "/";
		std::string canonicalQueryString = "";
		std::string canonicalHeaders = "content-type:application/json; charset=utf-8\nhost:" + host + "\n";
		std::string signedHeaders = "content-type;host";
		std::string hashedRequestPayload = Sha256(payload);
		std::string canonicalRequest = httpRequestMethod + "\n" + canonicalUri + "\n" + canonicalQueryString + "\n" + canonicalHeaders + "\n" + signedHeaders + "\n" + hashedRequestPayload;

		// Step 2: Create string to sign
		std::string credentialScope = date + "/" + service + "/tc3_request";
		std::string hashedCanonicalRequest = Sha256(canonicalRequest);
		std::string stringToSign = algorithm + "\n" + std::to_string(timestamp) + "\n" + credentialScope + "\n" + hashedCanonicalRequest;

		// Step 3: Calculate signature
		std::string secretId = WideStringToString(credentials->secretId);
		std::string secretKey = WideStringToString(credentials->secretKey);
		std::string signature = TC3Sign(secretKey, date, service, stringToSign);

		// Step 4: Build authorization header
		std::string authorization = algorithm + " Credential=" + secretId + "/" + credentialScope + ", SignedHeaders=" + signedHeaders + ", Signature=" + signature;

		// Prepare headers
		std::wstring headers = L"Content-Type: application/json; charset=utf-8\r\n";
		headers += L"Authorization: " + StringToWideString(authorization) + L"\r\n";
		headers += L"Host: " + StringToWideString(host) + L"\r\n";
		headers += L"X-TC-Action: " + StringToWideString(action) + L"\r\n";
		headers += L"X-TC-Timestamp: " + std::to_wstring(timestamp) + L"\r\n";
		headers += L"X-TC-Version: " + StringToWideString(version) + L"\r\n";
		headers += L"X-TC-Region: " + StringToWideString(region) + L"\r\n";

		if (HttpRequest httpRequest{
			L"Mozilla/5.0 Textractor",
			StringToWideString(host).c_str(),
			L"POST",
			L"/",
			payload,
			headers.c_str()
		}) {
			try {
				auto parsedJson = json::parse(WideStringToString(httpRequest.response));

				// Check for error in response
				if (parsedJson.contains("Response") && parsedJson["Response"].contains("Error"))
				{
					std::string errorCode = parsedJson["Response"]["Error"].value("Code", "Unknown");
					std::string errorMessage = parsedJson["Response"]["Error"].value("Message", "Unknown error");
					return { false, FormatString(L"%s (%s): %s", TRANSLATION_ERROR, StringToWideString(errorCode), StringToWideString(errorMessage)) };
				}

				// Get translation result
				if (parsedJson.contains("Response") && parsedJson["Response"].contains("TargetText"))
				{
					std::string targetText = parsedJson["Response"]["TargetText"].get<std::string>();
					if (!targetText.empty())
					{
						return { true, StringToWideString(targetText) };
					}
				}

				return { false, FormatString(L"%s: No translation found in response", TRANSLATION_ERROR) };
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
