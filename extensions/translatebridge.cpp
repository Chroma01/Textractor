#include "qtcommon.h"
#include "translatewrapperlite.h"
#include "network.h"

extern const wchar_t *TRANSLATION_ERROR;

const char *TRANSLATION_PROVIDER = "Translate Bridge";


bool translateSelectedOnly = false, useRateLimiter = true, rateLimitSelected = false, useCache = false, useFilter = true, enable = false;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 1000, port = 3005;

std::pair<bool, std::wstring> Translate(const std::wstring &text, TranslationParam tlp) {
    if (!enable) return {true, text};
    if (HttpRequest httpRequest{
        L"Mozilla/5.0 Textractor",
        L"localhost",
        L"POST",
        L"/translate",
        FormatString(R"({"text": "%s"})", JSON::Escape(WideStringToString(text))),
        L"Content-Type: application/json",
        DWORD(port),
        NULL,
        WINHTTP_FLAG_ESCAPE_DISABLE
    }) {
        if (auto translation = Copy(JSON::Parse(httpRequest.response)[L"translation"].String())) return {true, translation.value()};
        else return {false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response)};
    }

    else return {false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode)};
}
