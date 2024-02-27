#pragma once

// Enable more details for debugging
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "pch.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class IDXGIFactory4;

namespace ave {
	class D3DApp {
	protected:
		IDXGIFactory4* m_poFactory;
		IDXGISwapChain* m_poSwapChain;
		ID3D12Device* m_poDevice;

		ID3D12Fence* m_poFence;
		UINT64 m_iCurrentFence;

		ID3D12CommandQueue* m_oCommandQueue;
		ID3D12CommandAllocator* m_oDirectCmdListAlloc;
		ID3D12GraphicsCommandList* m_oCommandList;

		static const int M_ISWAP_CHAIN_BUFFER_COUNT = 2;
		int m_iCurrBackBuffer;
		ID3D12Resource* m_rSwapChainBuffer[M_ISWAP_CHAIN_BUFFER_COUNT];
		ID3D12Resource* m_rDepthStencilBuffer;

		ID3D12DescriptorHeap* m_oRtvHeap;
		ID3D12DescriptorHeap* m_oDsvHeap;

		D3D12_VIEWPORT m_oScreenViewport;
		D3D12_RECT m_oScissorRect;

		UINT m_iRtvDescriptorSize;
		UINT m_iDsvDescriptorSize;

		UINT m_i4xMsaaQuality;

		DXGI_FORMAT m_fBackBufferFormat;

	protected:
		void CreateRtvAndDsvDescriptorHeaps();

	protected:
		bool InitMainWindow();
		bool InitDirect3D();
		void CreateCommandObjects();
		void CreateSwapChain();

		void FlushCommandQueue();

		ID3D12Resource* CurrentBackBuffer()const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

		void CalculateFrameStats();

	public:
		D3DApp();
		D3DApp(const D3DApp& rhs) = delete;
		D3DApp& operator=(const D3DApp& rhs) = delete;
		virtual ~D3DApp();

		virtual bool Initialize(HINSTANCE hInstance);
	};
}