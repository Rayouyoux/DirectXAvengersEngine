#pragma once

namespace ave {

	namespace Time
	{
		// RETURNS TIME STRING
		// Normal Format = 'HH:MM:SS'
		// Stripped Format = 'HHMMSS'
		std::wstring GetTime(BOOL stripped = FALSE);

		// RETURNS DATE STRING
		// Normal Format = 'DD/MM/YY'
		// Stripped Format = 'DDMMYY'
		std::wstring GetDate(BOOL stripped = FALSE);

		// RETURNS DATE & TIME STRING
		// Normal Format = 'DD/MM/YY HH:MM:SS'
		// Stripped Format = 'DDMMYYHHMMSS'
		std::wstring GetDateTimeString(BOOL stripped = FALSE);
	};
}

