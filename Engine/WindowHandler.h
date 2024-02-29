#pragma once

#include <windows.h>

// Enable more details for debugging
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

namespace ave {
	class GraphicsHandler;
	class GameTime;

	class AvengersEngine
	{
	protected:
		HINSTANCE m_ohInstance;
		HWND m_oMainWnd;

		GraphicsHandler* m_poGraphics;
		GameTime* m_poTimer;

		std::wstring m_sMainWndCaption;
		int m_iClientWidth;
		int m_iClientHeight;

	protected:
		AvengersEngine(HINSTANCE hInstance);
		AvengersEngine(const AvengersEngine& ave) = delete;
		AvengersEngine& operator=(const AvengersEngine& ave) = delete;
		~AvengersEngine();

	protected:
		bool RegisterWndClass();
		bool CreateWnd();

	protected:
		static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		static AvengersEngine* Create(HINSTANCE hInstance);
		bool Initialize();

		virtual void Release();

	public:
		HINSTANCE GetHInstance() const { return m_ohInstance; }
		HWND GetWindow() const { return m_oMainWnd; }

		int GetWindowWidth() const { return m_iClientWidth; }
		int GetWindowHeight() const { return m_iClientHeight; }
	};
}