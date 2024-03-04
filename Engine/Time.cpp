#include "pch.h"
#include "Time.h"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace ave {

	std::wstring Time::GetTime(BOOL stripped)
	{
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);
		std::wstringstream wss;
		wss << std::put_time(&ltm, L"%T");

		std::wstring timeString = wss.str();

		if (stripped) {
			std::wstring chars = L":";
			for (WCHAR c : chars) {
				timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
			}
		}
		return timeString;
	}

	std::wstring Time::GetDate(BOOL stripped)
	{
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);
		std::wstringstream wss;
		wss << std::put_time(&ltm, L"%d/%m/%y");

		std::wstring timeString = wss.str();

		if (stripped) {
			std::wstring chars = L"/";
			for (WCHAR c : chars) {
				timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
			}
		}

		return timeString;
	}

	std::wstring Time::GetDateTimeString(BOOL stripped)
	{
		std::wstring timeString = GetDate(stripped) + L" " + GetTime(stripped);

		if (stripped) {
			std::wstring chars = L" ";
			for (WCHAR c : chars) {
				timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
			}
		}

		return timeString;
	}
}