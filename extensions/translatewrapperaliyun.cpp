#include "qtcommon.h"
#include "extension.h"
#include "translatewrapperaliyun.h"
#include "blockmarkup.h"
#include <concurrent_priority_queue.h>
#include <fstream>
#include <QComboBox>

#include "../texthook/main.h"

extern const char* NATIVE_LANGUAGE;
extern const char* TRANSLATE_TO;
extern const char* TRANSLATE_FROM;
extern const char* TRANSLATE_SELECTED_THREAD_ONLY;
extern const char* RATE_LIMIT_ALL_THREADS;
extern const char* RATE_LIMIT_SELECTED_THREAD;
extern const char* USE_TRANS_CACHE;
extern const char* FILTER_GARBAGE;
extern const char* MAX_TRANSLATIONS_IN_TIMESPAN;
extern const char* TIMESPAN;
extern const char* MAX_SENTENCE_SIZE;
extern const char* API_KEY;
extern const wchar_t* SENTENCE_TOO_LARGE_TO_TRANS;
extern const wchar_t* TRANSLATION_ERROR;
extern const wchar_t* TOO_MANY_TRANS_REQUESTS;
extern const char* TRANSLATE_EDITON;
extern const char* TRANSLATION_PROVIDER;
extern const char* GET_API_KEY_FROM;
extern const QStringList languagesTo, languagesFrom, version;
extern bool translateSelectedOnly, useRateLimiter, rateLimitSelected, useCache, useFilter;
extern int tokenCount, rateLimitTimespan, maxSentenceSize;

constexpr auto KEY_TRANSLATE_TO = u8"Translate to";
constexpr auto KEY_TRANSLATE_FROM = u8"Translate from";
constexpr auto KEY_TRANSLATE_SELECTED_THREAD_ONLY = u8"Translate selected text thread only";
constexpr auto KEY_RATE_LIMIT_ALL_THREADS = u8"Use rate limiter";
constexpr auto KEY_RATE_LIMIT_SELECTED_THREAD = u8"Rate limit selected text thread";
constexpr auto KEY_USE_TRANS_CACHE = u8"Use translation cache";
constexpr auto KEY_FILTER_GARBAGE = u8"Filter garbage characters";
constexpr auto KEY_MAX_TRANSLATIONS_IN_TIMESPAN = u8"Max translation requests in timespan";
constexpr auto KEY_TIMESPAN = u8"Timespan (ms)";
constexpr auto KEY_MAX_SENTENCE_SIZE = u8"Max sentence size";
constexpr auto KEY_API_KEY = u8"API key";
constexpr auto KEY_TRANSLATE_EDITON = u8"Translation Edition";

enum class Language;
extern Language CURRENT_LANGUAGE;
extern void Localize();

std::pair<bool, std::wstring> Translate(const std::wstring& text, TranslationParam tlp);

QFormLayout* display;
Settings settings;

namespace
{
	Synchronized<TranslationParam> tlp;
	Synchronized<std::unordered_map<std::wstring, std::wstring>> translationCache;

	std::string CacheFile()
	{
		return FormatString("%s Cache (%S).txt", TRANSLATION_PROVIDER, tlp->translateTo);
	}
	void SaveCache()
	{
		std::wstring allTranslations(L"\xfeff");
		for (const auto& [sentence, translation] : translationCache.Acquire().contents)
			allTranslations.append(L"|SENTENCE|").append(sentence).append(L"|TRANSLATION|").append(translation).append(L"|END|\r\n");
		std::ofstream(CacheFile(), std::ios::binary | std::ios::trunc).write((const char*)allTranslations.c_str(), allTranslations.size() * sizeof(wchar_t));
	}
	void LoadCache()
	{
		translationCache->clear();
		std::ifstream stream(CacheFile(), std::ios::binary);
		BlockMarkupIterator savedTranslations(stream, Array<std::wstring_view>{ L"|SENTENCE|", L"|TRANSLATION|" });
		auto translationCache = ::translationCache.Acquire();
		while (auto read = savedTranslations.Next())
		{
			auto& [sentence, translation] = read.value();
			translationCache->try_emplace(std::move(sentence), std::move(translation));
		}
	}
}

class Window : public QDialog, Localizer
{
public:
	Window() : QDialog(nullptr, Qt::WindowMinMaxButtonsHint)
	{
		QString iniPath = QCoreApplication::applicationDirPath() + "/Textractor.ini";
		QSettings settings0(iniPath, QSettings::IniFormat);
		int langId = settings0.value("Language", 0).toInt();
		CURRENT_LANGUAGE = static_cast<Language>(langId);
		Localize();

		display = new QFormLayout(this);

		settings.beginGroup(TRANSLATION_PROVIDER);

		auto translateToCombo = new QComboBox(this);
		translateToCombo->addItems(languagesTo);
		int i = -1;
		if (settings.contains(KEY_TRANSLATE_TO)) i = translateToCombo->findText(settings.value(KEY_TRANSLATE_TO).toString());
		if (i < 0) i = translateToCombo->findText(NATIVE_LANGUAGE, Qt::MatchStartsWith);
		if (i < 0) i = translateToCombo->findText("English", Qt::MatchStartsWith);
		translateToCombo->setCurrentIndex(i);
		SaveTranslateTo(translateToCombo->currentText());
		display->addRow(TRANSLATE_TO, translateToCombo);
		connect(translateToCombo, &QComboBox::currentTextChanged, this, &Window::SaveTranslateTo);

		auto translateFromCombo = new QComboBox(this);
		translateFromCombo->addItem("?");
		translateFromCombo->addItems(languagesFrom);
		i = -1;
		if (settings.contains(KEY_TRANSLATE_FROM)) i = translateFromCombo->findText(settings.value(KEY_TRANSLATE_FROM).toString());
		if (i < 0) i = 0;
		translateFromCombo->setCurrentIndex(i);
		SaveTranslateFrom(translateFromCombo->currentText());
		display->addRow(TRANSLATE_FROM, translateFromCombo);
		connect(translateFromCombo, &QComboBox::currentTextChanged, this, &Window::SaveTranslateFrom);

		auto translateVersionCombo = new QComboBox(this);
		translateVersionCombo->addItems(version);
		i = -1;
		if (settings.contains(KEY_TRANSLATE_EDITON)) i = translateVersionCombo->findText(settings.value(KEY_TRANSLATE_EDITON).toString());
		if (i < 0) i = 0;
		translateVersionCombo->setCurrentIndex(i);
		SaveTranslateVersion(translateVersionCombo->currentText());
		display->addRow(TRANSLATE_EDITON, translateVersionCombo);
		connect(translateVersionCombo, &QComboBox::currentTextChanged, this, &Window::SaveTranslateVersion);

		for (auto [value, label, keylabel] : Array<bool&, const char*, const char*>{
			{ translateSelectedOnly, TRANSLATE_SELECTED_THREAD_ONLY, KEY_TRANSLATE_SELECTED_THREAD_ONLY },
			{ useRateLimiter, RATE_LIMIT_ALL_THREADS, KEY_RATE_LIMIT_ALL_THREADS },
			{ rateLimitSelected, RATE_LIMIT_SELECTED_THREAD, KEY_RATE_LIMIT_SELECTED_THREAD },
			{ useCache, USE_TRANS_CACHE, KEY_USE_TRANS_CACHE },
			{ useFilter, FILTER_GARBAGE, KEY_FILTER_GARBAGE }
		})
		{
			value = settings.value(keylabel, value).toBool();
			auto checkBox = new QCheckBox(this);
			checkBox->setChecked(value);
			display->addRow(label, checkBox);
			connect(checkBox, &QCheckBox::clicked, [keylabel, &value](bool checked) { settings.setValue(keylabel, value = checked); });
		}
		for (auto [value, label, keylabel] : Array<int&, const char*, const char*>{
			{ tokenCount, MAX_TRANSLATIONS_IN_TIMESPAN, KEY_MAX_TRANSLATIONS_IN_TIMESPAN },
			{ rateLimitTimespan, TIMESPAN, KEY_TIMESPAN },
			{ maxSentenceSize, MAX_SENTENCE_SIZE, KEY_MAX_SENTENCE_SIZE },
		})
		{
			value = settings.value(keylabel, value).toInt();
			auto spinBox = new QSpinBox(this);
			spinBox->setRange(0, INT_MAX);
			spinBox->setValue(value);
			display->addRow(label, spinBox);
			connect(spinBox, qOverload<int>(&QSpinBox::valueChanged), [keylabel, &value](int newValue) { settings.setValue(keylabel, value = newValue); });
		}
		if (GET_API_KEY_FROM)
		{
			auto keyEdit = new QLineEdit(settings.value(KEY_API_KEY).toString(), this);
			tlp->authKey = S(keyEdit->text());
			QObject::connect(keyEdit, &QLineEdit::textChanged, [](QString key) { settings.setValue(KEY_API_KEY, S(tlp->authKey = S(key))); });
			auto keyLabel = new QLabel(QString("<a href=\"%1\">%2</a>").arg(GET_API_KEY_FROM, API_KEY), this);
			keyLabel->setOpenExternalLinks(true);
			display->addRow(keyLabel, keyEdit);
		}

		setWindowTitle(TRANSLATION_PROVIDER);
		QMetaObject::invokeMethod(this, &QWidget::show, Qt::QueuedConnection);
	}

	~Window()
	{
		SaveCache();
	}

private:
	void SaveTranslateTo(QString language)
	{
		SaveCache();
		settings.setValue(KEY_TRANSLATE_TO, S(tlp->translateTo = S(language)));
		LoadCache();
	}
	void SaveTranslateFrom(QString language)
	{
		settings.setValue(KEY_TRANSLATE_FROM, S(tlp->translateFrom = S(language)));
	}
	void SaveTranslateVersion(QString version)
	{
		settings.setValue(KEY_TRANSLATE_EDITON, S(tlp->translateVersion = S(version)));
	}
} window;

std::vector<std::wstring> splitWString(const std::wstring& sentence, const std::wstring& delimiter) {
	std::vector<std::wstring> result;
	size_t start = 0;
	size_t end = sentence.find(delimiter);

	while (end != std::wstring::npos) {
		result.push_back(sentence.substr(start, end - start));
		start = end + delimiter.length();
		end = sentence.find(delimiter, start);
	}

	result.push_back(sentence.substr(start));
	return result;
}

bool ProcessSentence(std::wstring &sentence, SentenceInfo sentenceInfo) {
    if (sentenceInfo["text number"] == 0) return false; // not processing the sentence from console

    static class {
    public:
        bool Request() {
            DWORD64 current = GetTickCount64(), token;
            while (tokens.try_pop(token))
                if (token > current - rateLimitTimespan) {
                    tokens.push(token); // popped one too many
                    break;
                }
            bool available = tokens.size() < tokenCount;
            if (available) tokens.push(current);
            return available;
        }

    private:
        concurrency::concurrent_priority_queue<DWORD64, std::greater<DWORD64> > tokens;
    } rateLimiter;

    bool cache = false;
    std::wstring translation;
    std::vector<std::wstring> parts = splitWString(sentence, L"\x200b \n");
    std::wstring &originalSentence = parts[0];

    if (useFilter) {
        Trim(originalSentence);
        originalSentence.erase(std::remove_if(originalSentence.begin(), originalSentence.end(),
                                              [](wchar_t ch) { return ch < ' ' && ch != '\n'; }), originalSentence.end());
    }
    if (originalSentence.empty()) return true;
    if (originalSentence.size() > maxSentenceSize) translation = SENTENCE_TOO_LARGE_TO_TRANS;
    if (useCache) {
        auto translationCache = ::translationCache.Acquire();
        if (auto it = translationCache->find(originalSentence); it != translationCache->end()) translation = it->second;
    }
    if (translation.empty() && (!translateSelectedOnly || sentenceInfo["current select"]))
        if (rateLimiter.Request() || !useRateLimiter || (!rateLimitSelected && sentenceInfo["current select"]))
            std::tie(cache, translation) = Translate(originalSentence, tlp.Copy());
        else translation = TOO_MANY_TRANS_REQUESTS;
    if (cache) translationCache->operator[](originalSentence) = translation;

    if (useFilter) Trim(translation);
    for (int i = 0; i < translation.size(); ++i) if (translation[i] == '\r' && translation[i + 1] == '\n')
        translation[i] = 0x200b; // for some reason \r appears as newline - no need to double
    if (translation.empty()) translation = TRANSLATION_ERROR;

    sentence = originalSentence;
    for (size_t i = 1; i < parts.size(); ++i) {
        sentence += L"\x200b \n" + parts[i];
    }
    sentence += L"\x200b \n" + translation;
    return true;
}

extern const std::unordered_map<std::wstring, std::wstring> codes;
TEST(
	{
		assert(Translate(L"こんにちは", { L"English", L"?", L"free", L"" }).second.find(L"ello") == 1 || strstr(TRANSLATION_PROVIDER, "DevTools"));
		for (auto languages : { languagesFrom, languagesTo }) for (auto language : languages)
			assert(codes.count(S(language)));
		assert(codes.count(L"?"));
	}
);
