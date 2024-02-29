#include "pch.h"
#include "WindowHandler.h"
#include "GraphicsHandler.h"

namespace ave {
	AvengersEngine::AvengersEngine(HINSTANCE hInstance) {
		m_ohInstance = nullptr;
		m_oMainWnd = nullptr;

		m_poGraphics = nullptr;
		m_poTimer = nullptr;

		m_sMainWndCaption = L"Avengers Game";
		m_iClientWidth = 800;
		m_iClientHeight = 600;
	}

	AvengersEngine::~AvengersEngine() {
		// Check how to release m_ohInstance;
		DestroyWindow(m_oMainWnd);

		m_poGraphics->Release();
		delete m_poTimer;
	}

	AvengersEngine* AvengersEngine::Create(HINSTANCE hInstance) {
		return new AvengersEngine(hInstance);
	}

	bool AvengersEngine::Initialize() {
		if (RegisterWndClass() == false) {
			return false;
		}
		
		if (CreateWnd() == false) {
			return false;
		}

		m_poGraphics = GraphicsHandler::Create();
		m_poGraphics->Initialize(this);

		m_poTimer = new GameTime();
		m_poTimer->Initialize();

		return true;
	}

	bool AvengersEngine::RegisterWndClass() {
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_ohInstance;
		wc.hIcon = LoadIcon(m_ohInstance, IDI_APPLICATION);
		wc.hCursor = LoadCursor(m_ohInstance, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"MainWnd";

		if (RegisterClass(&wc) == false)
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return false;
		}
	}

	bool AvengersEngine::CreateWnd() {
		RECT R = { 0, 0, m_iClientWidth, m_iClientHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_oMainWnd = CreateWindow(L"MainWnd", m_sMainWndCaption.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_ohInstance, 0);
		if (m_oMainWnd == nullptr) {
			MessageBox(0, std::to_wstring(GetLastError()).c_str(), 0, 0);
			return false;
		}

		ShowWindow(m_oMainWnd, SW_SHOW);
		UpdateWindow(m_oMainWnd);
	}

	LRESULT AvengersEngine::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		AvengersEngine* poAve = (AvengersEngine*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
		switch (msg) {
			// WM_ACTIVATE is sent when the window is activated or deactivated.  
			// We pause the game when the window is deactivated and unpause it 
			// when it becomes active.  
		case WM_ACTIVATE:
			//if (LOWORD(wParam) == WA_INACTIVE) {
			//	m_bAppPaused = true;
			//} else {
			//	m_bAppPaused = false;
			//	//mTimer.Start();
			//}
			return 0;

			// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			// Save the new client area dimensions.
			m_iClientWidth = LOWORD(lParam);
			m_iClientHeight = HIWORD(lParam);
			if (m_poDevice) {
				if (wParam == SIZE_MINIMIZED) {
					m_bAppPaused = true;
					m_bMinimized = true;
					m_bMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED) {
					m_bAppPaused = false;
					m_bMinimized = false;
					m_bMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED) {
					// Restoring from minimized state?
					if (m_bMinimized) {
						m_bAppPaused = false;
						m_bMinimized = false;
						OnResize();
					}

					// Restoring from maximized state?
					else if (m_bMaximized)
					{
						m_bAppPaused = false;
						m_bMaximized = false;
						OnResize();
					}
					else if (m_bResizing)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						OnResize();
					}
				}
			}
			return 0;

			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
			m_bAppPaused = true;
			m_bResizing = true;
			//mTimer.Stop();
			return 0;

			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			m_bAppPaused = false;
			m_bResizing = false;
			//mTimer.Start();
			OnResize();
			return 0;

			// WM_DESTROY is sent when the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

			// The WM_MENUCHAR message is sent when a menu is active and the user presses 
			// a key that does not correspond to any mnemonic or accelerator key. 
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
			else if ((int)wParam == VK_F2)
				Set4xMsaaState(!m_b4xMsaaState);

			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void AvengersEngine::Release() {
		delete this;
	}
}