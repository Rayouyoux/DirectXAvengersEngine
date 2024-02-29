#pragma once

// Enable more details for debugging
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "GameTime.h"
#include "Graphics.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace ave {
	class D3DApp {
	protected:
		static D3DApp* m_poApp; // Singleton Instance

		HINSTANCE m_oAppInst;
		HWND m_oMainWnd;
		bool m_bAppPaused;
		bool m_bMinimized;
		bool m_bMaximized;
		bool m_bResizing;
		bool m_bFullscreenState;

		// Set true to use 4X MSAA (§4.1.8).  The default is false.
		bool m_b4xMsaaState;    // 4X MSAA enabled
		UINT m_i4xMsaaQuality;  // quality level of 4X MSAA

		GameTime m_oTimer;
		GraphicsHandler m_oGraphics;

		IDXGIFactory4* m_poFactory;
		IDXGISwapChain* m_poSwapChain;
		ID3D12Device* m_poDevice;

		ID3D12Fence* m_poFence;
		UINT64 m_iCurrentFence;

		ID3D12CommandQueue* m_poCommandQueue;
		ID3D12CommandAllocator* m_poDirectCmdListAlloc;
		ID3D12GraphicsCommandList* m_poCommandList;

		static const int SWAP_CHAIN_BUFFER_COUNT = 2;
		int m_iCurrBackBuffer;
		ID3D12Resource* m_prSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
		ID3D12Resource* m_prDepthStencilBuffer;

		ID3D12DescriptorHeap* m_poRtvHeap;
		ID3D12DescriptorHeap* m_poDsvHeap;

		D3D12_VIEWPORT m_oScreenViewport;
		D3D12_RECT m_oScissorRect;

		UINT m_iRtvDescriptorSize;
		UINT m_iDsvDescriptorSize;

		std::wstring m_sMainWndCaption;
		D3D_DRIVER_TYPE m_eDriverType;
		DXGI_FORMAT m_eBackBufferFormat;
		DXGI_FORMAT m_eDepthStencilFormat;
		int m_iClientWidth;
		int m_iClientHeight;

	protected:
		D3DApp(HINSTANCE hInstance);
		D3DApp(const D3DApp& rhs) = delete;
		D3DApp& operator=(const D3DApp& rhs) = delete;
		virtual ~D3DApp();

	public:
		static D3DApp* GetApp();

		HINSTANCE AppInst() const;
		HWND MainWnd() const;
		float AspectRatio() const;
		ID3D12Device* GetDevice()const; 

		bool Get4xMsaaState() const;
		UINT Get4xMsaaQuality() const;
		void Set4xMsaaState(bool value);

		int Run();

		virtual bool Initialize();
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual void CreateRtvAndDsvDescriptorHeaps();
		virtual void OnResize();
		virtual void Update() = 0;
		virtual void Render() = 0;

		// TODO: TEMPORARY should be replaced with the Input class
		// Convenience overrides for handling mouse input.
		virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
		virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
		virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

	protected:
		bool InitMainWindow();
		bool InitDirect3D();
		void CreateCommandObjects();
		void CreateSwapChain();

		void FlushCommandQueue();

		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

		void CalculateFrameStats();
	};
}