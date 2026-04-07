#include <Psapi.h>

inline std::optional<std::wstring> GetModuleFilename(DWORD processId, HMODULE module = NULL)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	if (AutoHandle<> process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processId))
		if (GetModuleFileNameExW(process, module, buffer.data(), MAX_PATH)) return buffer.data();
	return {};
}

inline std::optional<std::wstring> GetModuleFilename(HMODULE module = NULL)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	if (GetModuleFileNameW(module, buffer.data(), MAX_PATH)) return buffer.data();
	return {};
}

inline bool IsProcess64Bit(DWORD processId)
{
	static bool isOS64Bit = []() {
		BOOL isWow64 = FALSE;
		IsWow64Process(GetCurrentProcess(), &isWow64);
		return isWow64 || x64;
	}();

	if (!isOS64Bit) return false;

	if (AutoHandle<> process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId))
	{
		BOOL isWow64 = FALSE;
		IsWow64Process(process, &isWow64);
		return !isWow64;
	}
	return false;
}

inline std::optional<std::wstring> GetProcessMainWindowTitle(DWORD processId)
{
	struct EnumData { DWORD targetPid; std::wstring windowTitle; };
	EnumData data{ processId, L"" };

	EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
	{
		EnumData* pData = (EnumData*)lParam;
		DWORD dwProcessId = 0;
		GetWindowThreadProcessId(hwnd, &dwProcessId);
		if (dwProcessId == pData->targetPid && IsWindowVisible(hwnd))
		{
			wchar_t title[1000] = {};
			GetWindowTextW(hwnd, title, sizeof(title) / sizeof(title[0]));
			if (wcslen(title) > 0)
			{
				pData->windowTitle = title;
				return FALSE;
			}
		}
		return TRUE;
	}, (LPARAM)&data);

	if (!data.windowTitle.empty()) return data.windowTitle;
	return {};
}

inline std::vector<std::pair<DWORD, std::optional<std::wstring>>> GetAllProcesses()
{
	std::vector<DWORD> processIds(10000);
	DWORD spaceUsed = 0;
	EnumProcesses(processIds.data(), 10000 * sizeof(DWORD), &spaceUsed);
	std::vector<std::pair<DWORD, std::optional<std::wstring>>> processes;
	for (int i = 0; i < spaceUsed / sizeof(DWORD); ++i) processes.push_back({ processIds[i], GetModuleFilename(processIds[i]) });
	return processes;
}
