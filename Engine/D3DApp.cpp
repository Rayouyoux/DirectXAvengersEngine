#include "pch.h"
#include "D3DApp.h"
#include <WindowsX.h>
#include <cassert>

namespace ave {

	D3DApp* D3DApp::mApp = nullptr;

	LRESULT CALLBACK
	MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
		//return D3DApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
	}

	D3DApp::D3DApp()
	{}

	D3DApp::~D3DApp()
	{}

	D3DApp* D3DApp::GetApp()
	{
		return mApp;
	}

	HINSTANCE D3DApp::AppInst()const
	{
		return mhAppInst;
	}

	HWND D3DApp::MainWnd()const
	{
		return mhMainWnd;
	}

	void D3DApp::Init(HINSTANCE hInstance) {
		mhAppInst = hInstance;
		assert(mApp == nullptr);
		mApp = this;
	}



	bool D3DApp::InitMainWindow()
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mhAppInst;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"Main Window";

		if (RegisterClass(&wc) == false)
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return false;
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, mClientWidth, mClientHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mhMainWnd = CreateWindow(L"MainWnd", mMainWndCaption.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);
		if (!mhMainWnd)
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
			return false;
		}

		ShowWindow(mhMainWnd, SW_SHOW);
		UpdateWindow(mhMainWnd);

		return true;
	}

	bool D3DApp::InitDirect3D()
	{
		return false;
	}

}