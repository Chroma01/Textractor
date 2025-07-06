#include "qtcommon.h"
#include "translatewrapper.h"
#include "devtools.h"

extern const wchar_t* ERROR_START_CHROME;
extern const wchar_t* TRANSLATION_ERROR;

const char* TRANSLATION_PROVIDER = "DevTools DeepL Translate";
const char* GET_API_KEY_FROM = nullptr;
std::wstring currTranslateTo;

extern const QStringList languagesTo
		{
			"Arabic",
			"Bulgarian",
			"Chinese (simplified)",
			"Chinese (traditional)",
			"Czech",
			"Danish",
			"Dutch",
			"English (American)",
			"English (British)",
			"Estonian",
			"Finnish",
			"French",
			"German",
			"Greek",
			"Hebrew",
			"Hungarian",
			"Indonesian",
			"Italian",
			"Japanese",
			"Korean",
			"Latvian",
			"Lithuanian",
			"Norwegian",
			"Polish",
			"Portuguese (Brazilian)",
			"Portuguese (target)",
			"Romanian",
			"Russian",
			"Slovak",
			"Slovenian",
			"Spanish",
			"Swedish",
			"Turkish",
			"Ukrainian",
			"Vietnamese"
		},
languagesFrom
{
	"Arabic",
	"Bulgarian",
	"Chinese",
	"Czech",
	"Danish",
	"Dutch",
	"English",
	"Estonian",
	"Finnish",
	"French",
	"German",
	"Greek",
	"Hebrew",
	"Hungarian",
	"Indonesian",
	"Italian",
	"Japanese",
	"Korean",
	"Latvian",
	"Lithuanian",
	"Norwegian",
	"Polish",
	"Portuguese",
	"Romanian",
	"Russian",
	"Slovak",
	"Slovenian",
	"Spanish",
	"Swedish",
	"Turkish",
	"Ukrainian",
	"Vietnamese"
};
extern const std::unordered_map<std::wstring, std::wstring> codes
{
	{{L"Arabic"}, {L"ar"}},
	{{L"Bulgarian"}, {L"bg"}},
	{{L"Chinese"}, {L"zh"}},
	{{L"Chinese (simplified)"}, {L"zh-hans"}},
	{{L"Chinese (traditional)"}, {L"zh-hant"}},
	{{L"Czech"}, {L"cs"}},
	{{L"Danish"}, {L"da"}},
	{{L"Dutch"}, {L"nl"}},
	{{L"German"}, {L"de"}},
	{{L"Greek"}, {L"el"}},
	{{L"English"}, {L"en"}},
	{{L"English (British)"}, {L"en-gb"}},
	{{L"English (American)"}, {L"en-us"}},
	{{L"Spanish"}, {L"es"}},
	{{L"Estonian"}, {L"et"}},
	{{L"Finnish"}, {L"fi"}},
	{{L"French"}, {L"fr"}},
	{{L"Hebrew"}, {L"he"}},
	{{L"Hungarian"}, {L"hu"}},
	{{L"Indonesian"}, {L"id"}},
	{{L"Italian"}, {L"it"}},
	{{L"Japanese"}, {L"ja"}},
	{{L"Korean"}, {L"ko"}},
	{{L"Lithuanian"}, {L"lt"}},
	{{L"Latvian"}, {L"lv"}},
	{{L"Norwegian"}, {L"nb"}},
	{{L"Polish"}, {L"pl"}},
	{{L"Portuguese"}, {L"pt"}},
	{{L"Portuguese (Brazilian)"}, {L"pt-br"}},
	{{L"Portuguese (target)"}, {L"pt-pt"}},
	{{L"Romanian"}, {L"ro"}},
	{{L"Russian"}, {L"ru"}},
	{{L"Slovak"}, {L"sk"}},
	{{L"Slovenian"}, {L"sl"}},
	{{L"Spanish"}, {L"es"}},
	{{L"Swedish"}, {L"sv"}},
	{{L"Turkish"}, {L"tr"}},
	{{L"Ukrainian"}, {L"uk"}},
	{{L"Vietnamese"}, {L"vi"}},

	{{L"?"}, {L"auto"}}

};

bool translateSelectedOnly = true, useRateLimiter = true, rateLimitSelected = false, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 2500;

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DevTools::Initialize();
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		DevTools::Close();
	}
	break;
	}
	return TRUE;
}

std::wstring htmlDecode (std::wstring text) {
    const std::wstring enc[] = { L"&amp;", L"&lt;", L"&gt;" };
    const std::wstring dec[] = { L"&", L"<", L">" };
	
	size_t pos;
	for(int j = 0; j < 3; j++) {
		do {
			pos = text.find(enc[j]);
	  		if (pos != std::wstring::npos)
		    		text.replace (pos,enc[j].length(),dec[j]);
    		} while (pos != std::wstring::npos);
  	}
	return text;
}

std::pair<bool, std::wstring> Translate(const std::wstring& text, TranslationParam tlp)
{
	if (!DevTools::Connected()) return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, ERROR_START_CHROME) };
	// DevTools can't handle concurrent translations yet
	static std::mutex translationMutex;
	std::scoped_lock lock(translationMutex);
	std::wstring escaped; // DeepL breaks with slash in input
	for (auto ch : text) ch == '/' ? escaped += L"\\/" : escaped += ch;
	DevTools::SendRequest("Runtime.evaluate",LR"({"expression":"document.querySelector('[data-testid=translator-target-input] div[contenteditable=true]').innerHTML = '';","returnByValue":false})");
	DevTools::SendRequest("Page.navigate", FormatString(LR"({"url":"https://www.deepl.com/en/translator#%s/%s/%s"})", (tlp.translateFrom == L"?") ? codes.at(tlp.translateFrom) : codes.at(tlp.translateFrom).substr(0, 2), codes.at(tlp.translateTo), Escape(escaped)));
	// if (currTranslateTo == tlp.translateTo)
	// 	DevTools::SendRequest("Page.navigate", FormatString(LR"({"url":"https://www.deepl.com/en/translator#%s/%s/%s"})", (tlp.translateFrom == L"?") ? codes.at(tlp.translateFrom) : codes.at(tlp.translateFrom).substr(0, 2), codes.at(tlp.translateTo).substr(0, 2), Escape(escaped)));
	// else
	// {
	// 	currTranslateTo = tlp.translateTo;
	// 	DevTools::SendRequest("Page.navigate", FormatString(LR"({"url":"https://www.deepl.com/en/translator#%s/%s/%s"})", (tlp.translateFrom == L"?") ? codes.at(tlp.translateFrom) : codes.at(tlp.translateFrom).substr(0, 2), codes.at(tlp.translateTo), Escape(escaped)));
	// }

	for (int retry = 0; ++retry < 100; Sleep(100))
		if (auto translation = Copy(DevTools::SendRequest("Runtime.evaluate",
			LR"({"expression":"document.querySelector('[data-testid=translator-target-input]').textContent.trim() ","returnByValue":true})"
		)[L"result"][L"value"].String()))
			if (!translation->empty()) {
				return { true, htmlDecode(translation.value()) };
			}
	if (auto errorMessage = Copy(DevTools::SendRequest("Runtime.evaluate",
		LR"({"expression":"document.querySelector('div.lmt__system_notification').innerHTML","returnByValue":true})"
	)[L"result"][L"value"].String())) return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, errorMessage.value()) };
	return { false, TRANSLATION_ERROR };
}
