#pragma once

namespace ave {

	class Logger
	{
	private:
		static Logger* inst;

	public:
		static Logger* Instance() { return inst; }

	public:

		// Constructor
		Logger();

		// Deconstructor
		~Logger();

		// Prints to Log File
		static void PrintLog(const WCHAR* fmt, ...);

		static std::wstring LogDirectory();

		static std::wstring LogFile();

		// Prints Line of '-' char's
		static void PrintDebugSeperator();

		// Returns true if MTail Running
		static bool IsMTailRunning();

		// Starts MTail Application
		static void StartMTail();
	};
}

