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
	{
		m_iRtvDescriptorSize = 0;
		m_iDsvDescriptorSize = 0;
		
		m_iCurrentFence = 0;

		m_iCurrBackBuffer = 0;

		m_i4xMsaaQuality = 0;
		m_fBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

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
#if defined(DEBUG) || defined(_DEBUG) 
		// Enable the D3D12 debug layer.
		{
			ID3D12Debug* debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
#endif
	
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_poFactory)));

		// Try to create hardware device.
		HRESULT hardwareResult = D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_poDevice));

		// Fallback to WARP device.
		if (FAILED(hardwareResult))
		{
			IDXGIAdapter* oWarpAdapter;
			ThrowIfFailed(m_poFactory->EnumWarpAdapter(IID_PPV_ARGS(&oWarpAdapter)));

			ThrowIfFailed(D3D12CreateDevice(
				oWarpAdapter,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_poDevice)));
		}

		ThrowIfFailed(m_poDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_poFence)));

		m_iRtvDescriptorSize = m_poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_iDsvDescriptorSize = m_poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		// CBV is used only for textures

		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = m_fBackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(m_poDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)));

		m_i4xMsaaQuality = msQualityLevels.NumQualityLevels;
		assert(m_i4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
		
		return false;
	}

	void D3DApp::CreateRtvAndDsvDescriptorHeaps()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = M_ISWAP_CHAIN_BUFFER_COUNT;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_poDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(&m_oRtvHeap)));

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_poDevice->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(&m_oDsvHeap)));
	}

	void D3DApp::CalculateFrameStats()
	{}
}