#pragma once

struct TranslationParam
{
	std::wstring translateTo, translateFrom, authKey;
	std::wstring provider, model, apiHost, apiPath, systemPrompt;
	double temperature;
	bool includePreviousContext = false;
	std::wstring previousSentence1;
	std::wstring previousSentence2;
};

