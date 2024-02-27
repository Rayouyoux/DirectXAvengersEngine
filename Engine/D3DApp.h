#pragma once

namespace ave {
	class D3DApp {
	protected:

	public:
		D3DApp();
		D3DApp(const D3DApp& rhs) = delete;
		D3DApp& operator=(const D3DApp& rhs) = delete;
		virtual ~D3DApp();

		void Init(HINSTANCE hInstance);
		bool InitMainWindow();
		bool InitDirect3D();
	};
}