#pragma once
#include <string>

namespace ave {
	class D3DApp {
	protected:

	public:
		D3DApp();
		D3DApp(const D3DApp& rhs) = delete;
		D3DApp& operator=(const D3DApp& rhs) = delete;
		virtual ~D3DApp();

		// Getter 
		static D3DApp* GetApp();
		HINSTANCE AppInst()const;
		HWND      MainWnd()const;

		void Init(HINSTANCE hInstance);
		bool InitMainWindow();
		bool InitDirect3D();

	protected:

		static D3DApp* mApp;

		HINSTANCE mhAppInst = nullptr;
		HWND      mhMainWnd = nullptr;

		std::wstring mMainWndCaption = L"d3d App";
		int mClientWidth = 800;
		int mClientHeight = 600;
	};
}