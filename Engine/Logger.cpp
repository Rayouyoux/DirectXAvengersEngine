#include "pch.h"
#include "Logger.h"
#include "GlobalTime.h"
#include <fstream>
#include <Shlobj.h>
#include <cstdio>
#include <tlhelp32.h>


namespace ave {

	Logger* Logger::inst;

	Logger::Logger()
	{
		inst = this;
	}

	Logger::~Logger()
	{
	}

	void Logger::PrintLog(const WCHAR* fmt, ...)
	{
		WCHAR buf[4096];
		va_list args;

		va_start(args, fmt);
		vswprintf_s(buf, fmt, args);
		va_end(args);

		//MessageBox(0, buf, 0, 0);
		OutputDebugString(buf);

		// Example File and Location = %AppData%/BlankProject/Log/BlankProject03052021194056.log

		std::wfstream outfile;

		outfile.open(std::wstring(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

		if (outfile.is_open()) {
			std::wstring s = buf;
			outfile << L"[" << GlobalTime::GetDateTimeString() << L"]  " << s;
			outfile.close();
			OutputDebugString(s.c_str());
		}
		else {
			MessageBox(NULL, L"Unable to open log file...", L"Log Error", MB_OK);
		}
	}

	std::wstring Logger::LogDirectory()
	{
		WCHAR Path[1024];
		WCHAR* AppDataLocal;
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &AppDataLocal);
		wcscpy_s(Path, AppDataLocal);
		wcscat_s(Path, L"\\");
		wcscat_s(Path, L"GAMENAME");
		CreateDirectory(Path, NULL);
		wcscat_s(Path, L"\\Log");
		CreateDirectory(Path, NULL);
		return Path;
	}

	std::wstring Logger::LogFile()
	{
		WCHAR File[1024];
		wcscpy_s(File, L"GAMENAME");
		wcscat_s(File, L"BOOTTIME");
		wcscat_s(File, L".log");
		return File;
	}

	void Logger::PrintDebugSeperator()
	{
		std::wstring s = L"\n------------------------------------------------------------------------------------\n\n";

		#ifdef _DEBUG
			std::wfstream outfile;
			outfile.open(std::wstring(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

			if (outfile.is_open()) {
				outfile << s;
				outfile.close();
			}
			else {
				MessageBox(NULL, L"Unable to open log file...", L"Log Error", MB_OK);
			}
		#endif
	}

	bool Logger::IsMTailRunning()
	{
		bool exists = false;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry))
			while (Process32Next(snapshot, &entry))
				if (!_wcsicmp(entry.szExeFile, L"mTAIL.exe"))
					exists = true;

		CloseHandle(snapshot);
		return exists;
	}

	void Logger::StartMTail()
	{
		if (IsMTailRunning()) {
			Logger::PrintLog(L"--MTail failed to start - Already Running\n");
			return;
		}

		Logger::PrintLog(L"--Starting MTail\n");
		WCHAR path[MAX_PATH] = { 0 };
		GetCurrentDirectoryW(MAX_PATH, path);
		std::wstring url = path + std::wstring(L"/mTAIL.exe");
		std::wstring params = L" \"" + LogDirectory() + L"/" + LogFile() + L"\" /start";
		ShellExecute(0, NULL, url.c_str(), params.c_str(), NULL, SW_SHOWDEFAULT);
	}
}