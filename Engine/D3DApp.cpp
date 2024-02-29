#include "pch.h"

namespace ave {
	LRESULT CALLBACK
	MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Forward hwnd on because we can get messages (e.g., WM_CREATE)
		// before CreateWindow returns, and thus before mhMainWnd is valid.
		return D3DApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
	}

	D3DApp* D3DApp::m_poApp = nullptr;
	D3DApp* D3DApp::GetApp() {
		return m_poApp;
	}

	D3DApp::D3DApp(HINSTANCE hInstance) {
		m_oAppInst = hInstance;
		assert(m_poApp == nullptr);
		m_poApp = this;

		m_oMainWnd = nullptr;
		m_bAppPaused = false;
		m_bMinimized = false;
		m_bMaximized = false;
		m_bResizing = false;
		m_bFullscreenState = false;

		m_b4xMsaaState = false;
		m_i4xMsaaQuality = 0;

		m_poFactory = nullptr;
		m_poSwapChain = nullptr;
		m_poDevice = nullptr;

		m_poFence = nullptr;
		m_iCurrentFence = 0;

		m_poCommandQueue = nullptr;
		m_poDirectCmdListAlloc = nullptr;
		m_poCommandList = nullptr;

		m_iCurrBackBuffer = 0;
		m_prDepthStencilBuffer = nullptr;

		m_poRtvHeap = nullptr;
		m_poDsvHeap = nullptr;

		m_iRtvDescriptorSize = 0;
		m_iDsvDescriptorSize = 0;

		m_sMainWndCaption = L"Avengers App";
		m_eDriverType = D3D_DRIVER_TYPE_HARDWARE;
		m_eBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_eDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_iClientWidth = 800;
		m_iClientHeight = 600;
	}

	D3DApp::~D3DApp() {
		if (m_poDevice != nullptr)
			FlushCommandQueue();

		// m_oAppInst; HOW TO DESTROY ?
		DestroyWindow(m_oMainWnd);
		m_poFactory->Release();
		m_poDirectCmdListAlloc->Release();
		m_poCommandList->Release();

		for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			m_prSwapChainBuffer[i]->Release();
		}
		m_prDepthStencilBuffer->Release();

		m_poRtvHeap->Release();
		m_poDsvHeap->Release();
	}

	HINSTANCE D3DApp::AppInst() const {
		return m_oAppInst;
	}

	HWND D3DApp::MainWnd() const {
		return m_oMainWnd;
	}

	float D3DApp::AspectRatio() const {
		return static_cast<float>(m_iClientWidth / m_iClientHeight);
	}

	bool D3DApp::Get4xMsaaState() const {
		return m_b4xMsaaState;
	}
	UINT D3DApp::Get4xMsaaQuality() const {
		return m_i4xMsaaQuality;
	}
	ID3D12Device* D3DApp::GetDevice()const {
		return m_poDevice;
	}

	void D3DApp::Set4xMsaaState(bool value) {
		if (m_b4xMsaaState != value) {
			m_b4xMsaaState = value;

			CreateSwapChain();
			OnResize();
		}
	}

	LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

	void D3DApp::OnResize() {
		assert(m_poDevice);
		assert(m_poSwapChain);
		assert(m_poDirectCmdListAlloc);

		FlushCommandQueue();

		ThrowIfFailed(m_poCommandList->Reset(m_poDirectCmdListAlloc, nullptr));

		// Clear the previous resources we will be recreating.
		for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			if (m_prSwapChainBuffer[i] != nullptr)
				m_prSwapChainBuffer[i]->Release();
		}
		if(m_prDepthStencilBuffer != nullptr)
			m_prDepthStencilBuffer->Release();

		// Resize the swap chain.
		ThrowIfFailed(m_poSwapChain->ResizeBuffers(
			SWAP_CHAIN_BUFFER_COUNT,
			m_iClientWidth, m_iClientHeight,
			m_eBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		m_iCurrBackBuffer = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_poRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		{
			ThrowIfFailed(m_poSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_prSwapChainBuffer[i])));
			m_poDevice->CreateRenderTargetView(m_prSwapChainBuffer[i], nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, m_iRtvDescriptorSize);
		}

		// Create the depth/stencil buffer and view.
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = m_iClientWidth;
		depthStencilDesc.Height = m_iClientHeight;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

		depthStencilDesc.SampleDesc.Count = m_b4xMsaaState ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_b4xMsaaState ? (m_i4xMsaaQuality - 1) : 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = m_eDepthStencilFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;
		CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(m_poDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(&m_prDepthStencilBuffer)));

		// Create descriptor to mip level 0 of entire resource using the format of the resource.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = m_eDepthStencilFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		m_poDevice->CreateDepthStencilView(m_prDepthStencilBuffer, &dsvDesc, DepthStencilView());

		// Transition the resource from its initial state to be used as a depth buffer.
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_prDepthStencilBuffer,
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		m_poCommandList->ResourceBarrier(1, &resourceBarrier);

		// Execute the resize commands.
		ThrowIfFailed(m_poCommandList->Close());
		ID3D12CommandList* cmdsLists[] = { m_poCommandList };
		m_poCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until resize is complete.
		FlushCommandQueue();

		// Update the viewport transform to cover the client area.
		m_oScreenViewport.TopLeftX = 0;
		m_oScreenViewport.TopLeftY = 0;
		m_oScreenViewport.Width = static_cast<float>(m_iClientWidth);
		m_oScreenViewport.Height = static_cast<float>(m_iClientWidth);
		m_oScreenViewport.MinDepth = 0.0f;
		m_oScreenViewport.MaxDepth = 1.0f;

		m_oScissorRect = { 0, 0, m_iClientWidth, m_iClientWidth };
	}

	bool D3DApp::Initialize() {
		if (InitMainWindow() == false)
			return false;
	
		if (InitDirect3D() == false)
			return false;

		m_oTimer.Initialize();
		OnResize();

		return true;
	}

	bool D3DApp::InitMainWindow() {
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_oAppInst;
		wc.hIcon = LoadIcon(m_oAppInst, IDI_APPLICATION);
		wc.hCursor = LoadCursor(m_oAppInst, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"MainWnd";

		if (RegisterClass(&wc) == false)
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
			return false;
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, m_iClientWidth, m_iClientHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_oMainWnd = CreateWindow(L"MainWnd", m_sMainWndCaption.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_oAppInst, 0);
		if (m_oMainWnd == nullptr) {
			MessageBox(0, std::to_wstring(GetLastError()).c_str(), 0, 0);
			return false;
		}

		ShowWindow(m_oMainWnd, SW_SHOW);
		UpdateWindow(m_oMainWnd);

		return true;
	}

	bool D3DApp::InitDirect3D() {
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
		msQualityLevels.Format = m_eBackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(m_poDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)));

		m_i4xMsaaQuality = msQualityLevels.NumQualityLevels;
		assert(m_i4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
		
		CreateCommandObjects();
		CreateSwapChain();
		CreateRtvAndDsvDescriptorHeaps();

		return true;
	}

	// TODO: Transform this part into a State Machine for paused etc...
	int D3DApp::Run() {
		MSG msg = { 0 };

		while (msg.message != WM_QUIT) {
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// Otherwise, do animation/game stuff.
			else {
				m_oTimer.Tick();

				if (m_bAppPaused == false) {
					CalculateFrameStats();
					Update();
					Render();
				}
				else {
					Sleep(100);
				}
			}
		}

		return static_cast<int>(msg.wParam);
	}

	void D3DApp::CreateCommandObjects() {
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_poDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_poCommandQueue)));

		ThrowIfFailed(m_poDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_poDirectCmdListAlloc)));

		ThrowIfFailed(m_poDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_poDirectCmdListAlloc, // Associated command allocator
			nullptr,                // Initial PipelineStateObject
			IID_PPV_ARGS(&m_poCommandList)));

		// Start off in a closed state.  This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		m_poCommandList->Close();
	}

	void D3DApp::CreateSwapChain() {
		// Release the previous swapchain we will be recreating.
		m_poSwapChain = nullptr;

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_iClientWidth;
		sd.BufferDesc.Height = m_iClientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = m_eBackBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = m_b4xMsaaState ? 4 : 1;
		sd.SampleDesc.Quality = m_b4xMsaaState ? (m_i4xMsaaQuality - 1) : 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
		sd.OutputWindow = m_oMainWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		ThrowIfFailed(m_poFactory->CreateSwapChain(
			m_poCommandQueue,
			&sd,
			&m_poSwapChain));
	}

	void D3DApp::FlushCommandQueue() {
		// Advance the fence value to mark commands up to this fence point.
		m_iCurrentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		ThrowIfFailed(m_poCommandQueue->Signal(m_poFence, m_iCurrentFence));

		// Wait until the GPU has completed commands up to this fence point.
		if (m_poFence->GetCompletedValue() < m_iCurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, L"false", false, EVENT_ALL_ACCESS);

			// Fire event when GPU hits current fence.  
			ThrowIfFailed(m_poFence->SetEventOnCompletion(m_iCurrentFence, eventHandle));

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	void D3DApp::CreateRtvAndDsvDescriptorHeaps() {
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_poDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(&m_poRtvHeap)));

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		ThrowIfFailed(m_poDevice->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(&m_poDsvHeap)));
	}

	ID3D12Resource* D3DApp::CurrentBackBuffer() const {
		return m_prSwapChainBuffer[m_iCurrBackBuffer];
	}

	D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_poRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_iCurrBackBuffer,
			m_iRtvDescriptorSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView() const
	{
		return m_poDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	void D3DApp::CalculateFrameStats()
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if ((m_oTimer.TotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;

			std::wstring fpsStr = std::to_wstring(fps);
			std::wstring mspfStr = std::to_wstring(mspf);

			std::wstring windowText = m_sMainWndCaption +
				L"    fps: " + fpsStr +
				L"   mspf: " + mspfStr;

			SetWindowText(m_oMainWnd, windowText.c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}
}