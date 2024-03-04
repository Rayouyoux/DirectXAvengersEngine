#include "pch.h"
#include <dxgi1_4.h>
#include <dxgi.h>
#include <sstream>
#include "GraphicsHandler.h"
#include "WindowHandler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Logger.h"
#include "Transform.h"
#include "Maths.h"
#include "ConstantsStruct.h"
#include "ParticleSystem.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


std::wstring RACISTEXMFLOAT4X4ToString(const DirectX::XMFLOAT4X4& matrix)
{
	std::wstringstream ss;
	ss << "[" << matrix._11 << ", " << matrix._12 << ", " << matrix._13 << ", " << matrix._14 << "]" << std::endl;
	ss << "[" << matrix._21 << ", " << matrix._22 << ", " << matrix._23 << ", " << matrix._24 << "]" << std::endl;
	ss << "[" << matrix._31 << ", " << matrix._32 << ", " << matrix._33 << ", " << matrix._34 << "]" << std::endl;
	ss << "[" << matrix._41 << ", " << matrix._42 << ", " << matrix._43 << ", " << matrix._44 << "]" << std::endl;
	return ss.str();
}

namespace ave {
	ID3D12GraphicsCommandList* GraphicsHandler::m_poCommandList;

	GraphicsHandler::GraphicsHandler() {
		m_poAve = nullptr;

		m_b4xMsaaState = false;
		m_i4xMsaaQuality = 0;

		m_poFactory = nullptr;
		m_poSwapChain = nullptr;
		GraphicsHandler::m_poDevice = nullptr;

		m_poFence = nullptr;
		m_iCurrentFence = 0;

		m_poCommandQueue = nullptr;
		m_poDirectCmdListAlloc = nullptr;
		GraphicsHandler::m_poCommandList = nullptr;

		m_iCurrBackBuffer = 0;
		m_prDepthStencilBuffer = nullptr;

		m_poRtvHeap = nullptr;
		m_poDsvHeap = nullptr;

		m_iRtvDescriptorSize = 0;
		m_iDsvDescriptorSize = 0;

		m_oScreenViewport = D3D12_VIEWPORT();
		m_oScissorRect = D3D12_RECT();

		m_cFillColor = DirectX::Colors::IndianRed;
		m_eDriverType = D3D_DRIVER_TYPE_HARDWARE;
		m_eBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_eDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	GraphicsHandler::~GraphicsHandler() {
		if (m_poDevice != nullptr)
			FlushCommandQueue();

		m_poFactory->Release();
		m_poDirectCmdListAlloc->Release();
		m_poCommandList->Release();

		for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			m_prSwapChainBuffer[i]->Release();
		}
		m_prDepthStencilBuffer->Release();

		m_poRtvHeap->Release();
		m_poDsvHeap->Release();

		delete m_poBehaviour;
	}


	GraphicsHandler* GraphicsHandler::Create() {
		return new GraphicsHandler();
	}

	bool GraphicsHandler::Initialize(AvengersEngine* poAve) {
#if defined(DEBUG) || defined(_DEBUG) // Enable the D3D12 debug layer.
		{
			ID3D12Debug* debugController;
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
				return false;
			}
			debugController->EnableDebugLayer();
		}
#endif
		m_poAve = poAve;

		m_poShader = new Shader();
		m_poMesh = new Mesh();

		m_poCameraEntity = new Entity();
		m_poCameraEntity->Start();

		m_poCubeEntity = new Entity();
		m_poCubeEntity->Start();
		float x = 5.0f * sinf(XM_PIDIV4) * cosf(1.5f * Maths::PI);
		float z = 5.0f * sinf(XM_PIDIV4) * sinf(1.5f * Maths::PI);
		float y = 5.0f * cosf(XM_PIDIV4);

		// Build the view matrix.
		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_view = XMMatrixLookAtLH(pos, target, up);
		//DirectX::XMFLOAT3 mInvLook = { 0.0f, 0.0f, 10.0f }; // A INCLURE DANS LE NAMESPACE Maths / UTILS
		//m_poCubeEntity->m_poTransform->Move(&mInvLook);

		m_poCamera = m_poCameraEntity->AddComponent<Camera>();
		m_poCamera->Start();
		/*std::wstring magic = RACISTEXMFLOAT4X4ToString(m_poCamera->GetProjectionMatrix());
		Logger::PrintLog(magic.c_str());*/

		MeshRenderer* poMeshRenderer = m_poCubeEntity->AddComponent<MeshRenderer>();
		poMeshRenderer->SetMesh(m_poMesh);
		poMeshRenderer->SetShader(m_poShader);

		m_poBehaviour = new Particles::ParticleBehaviour();
		m_poParticleSystem = m_poCubeEntity->AddComponent<Particles::ParticleSystem>();
		m_poParticleSystem->Initialize(10, 300);
		
		m_poParticleSystem->SetBehaviour(m_poBehaviour);
		m_poParticleSystem->SetMesh(m_poMesh);
		m_poParticleSystem->SetShader(m_poShader);

		bool test = CreateFactory()
			&& CreateDevice()
			&& CreateFence()
			&& RequestMsaaQuality()
			&& CreateCommandObjects() 
			&& CreateSwapChain()
			&& CreateRtv_Dsv_CBVDescriptorHeaps();

		if (FAILED(m_poCommandList->Reset(m_poDirectCmdListAlloc, nullptr))) {
			return false;
		}

		bool test2 = m_poShader->CreateShader(this)
			&& m_poMesh->BuildBoxGeometry(GetDevice(), GetCommandList());

		CloseCommandList();
		QueueCommandList();
		FlushCommandQueue();

		return test && test2;
	}

	void GraphicsHandler::OnResize() {
		if (m_poDevice == nullptr
			|| m_poCommandList == nullptr
			|| m_poDirectCmdListAlloc == nullptr)
			return;

		FlushCommandQueue();

		if (FAILED(m_poCommandList->Reset(m_poDirectCmdListAlloc, nullptr))) {
			return;
		}

		// Clear the previous resources we will be recreating.
		for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) {
			if (m_prSwapChainBuffer[i] != nullptr)
				m_prSwapChainBuffer[i]->Release();
		}
		if (m_prDepthStencilBuffer != nullptr)
			m_prDepthStencilBuffer->Release();

		// Resize the swap chain.
		if (FAILED(m_poSwapChain->ResizeBuffers(
			SWAP_CHAIN_BUFFER_COUNT,
			m_poAve->GetWindowWidth(),
			m_poAve->GetWindowHeight(),
			m_eBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))) {
			return;
		}

		m_iCurrBackBuffer = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_poRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		{
			if (FAILED(m_poSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_prSwapChainBuffer[i])))) {
				return;
			}
			m_poDevice->CreateRenderTargetView(m_prSwapChainBuffer[i], nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, m_iRtvDescriptorSize);
		}

		// Create the depth/stencil buffer and view.
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = m_poAve->GetWindowWidth();
		depthStencilDesc.Height = m_poAve->GetWindowHeight();
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
		if (FAILED(m_poDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(&m_prDepthStencilBuffer)))) {
			return;
		}

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
		if (FAILED(m_poCommandList->Close())) {
			return;
		}
		ID3D12CommandList* cmdsLists[] = { m_poCommandList };
		m_poCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until resize is complete.
		FlushCommandQueue();

		// Update the viewport transform to cover the client area.
		m_oScreenViewport.TopLeftX = 0;
		m_oScreenViewport.TopLeftY = 0;
		m_oScreenViewport.Width = static_cast<float>(m_poAve->GetWindowWidth());
		m_oScreenViewport.Height = static_cast<float>(m_poAve->GetWindowHeight());
		m_oScreenViewport.MinDepth = 0.0f;
		m_oScreenViewport.MaxDepth = 1.0f;

		m_oScissorRect = { 0, 0, m_poAve->GetWindowWidth(), m_poAve->GetWindowHeight() };
	}

	void GraphicsHandler::Update(float deltaTime) {
		/*m_poCameraEntity->Update(deltaTime);*/
		/*m_poCubeEntity->Update(deltaTime);*/

		/*m_poCameraEntity->m_poTransform->GetWorld()*/

		XMMATRIX world = m_poCubeEntity->m_poTransform->GetWorld();
		//XMMATRIX view = m_poCameraEntity->m_poTransform->GetWorld();
		
		

		float rot = XMConvertToRadians(45.0f * deltaTime);
		/*XMFLOAT3 rotate = { 0.0f ,rot, 0.0f };
		XMVECTOR rotateeee = XMLoadFloat3(&rotate);*/
		

		/*XMFLOAT3 scale = { -0.5f * deltaTime, -0.5f * deltaTime , -0.5f * deltaTime };*/

		/*XMFLOAT3 pos = { 0.005f * deltaTime, 0.005f * deltaTime , 0.005f * deltaTime };*/

		m_poCubeEntity->m_poTransform->RotateOnUp(rot);
		/*m_poCubeEntity->m_poTransform->Scale(&scale);*/
		/*m_poCubeEntity->m_poTransform->Move(&pos);*/

		m_poCubeEntity->m_poTransform->UpdateMatrice();

		XMMATRIX view = m_view;
		XMMATRIX proj = m_poCamera->GetProjectionMatrix();

		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
		m_poShader->UpdateObject(objConstants);

		PassConstants passConstants;
		XMStoreFloat4x4(&passConstants.View, XMMatrixTranspose(view));
		XMStoreFloat4x4(&passConstants.Proj, XMMatrixTranspose(proj));
		m_poShader->UpdatePass(passConstants);

		m_poParticleSystem->Update(deltaTime);
	}

	void GraphicsHandler::LateUpdate() {
		// Do with the Objects
	}

	void GraphicsHandler::Render() {
		RenderBegin();

		m_poCubeEntity->Render();
		// Add your coubeh

		m_poParticleSystem->Render();

		RenderCease();
	}

#pragma region Initializers

	bool GraphicsHandler::CreateFactory() {
		return SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&m_poFactory)));
	}

	bool GraphicsHandler::CreateDevice() {
		// Try to create hardware device.
		HRESULT hardwareResult = D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_poDevice));
		// Fallback to WARP device.
		if (FAILED(hardwareResult))
		{
			IDXGIAdapter* oWarpAdapter;
			if (FAILED(m_poFactory->EnumWarpAdapter(IID_PPV_ARGS(&oWarpAdapter)))) {
				return false;
			}

			if (FAILED(D3D12CreateDevice(
				oWarpAdapter,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_poDevice)))) {
				return false;
			}
		}

		m_iRtvDescriptorSize = m_poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_iDsvDescriptorSize = m_poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		// CBV is used only for textures
	}

	bool GraphicsHandler::CreateFence() {
		return (SUCCEEDED(m_poDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_poFence))));
	}

	bool GraphicsHandler::RequestMsaaQuality() {
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = m_eBackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;
		if (FAILED(m_poDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)))) {
			return false;
		}

		m_i4xMsaaQuality = msQualityLevels.NumQualityLevels;
		if (m_i4xMsaaQuality <= 0)
			return false;// Unexpected MSAA quality level

		return true;
	}

	bool GraphicsHandler::CreateCommandObjects() {
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		if (FAILED(m_poDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_poCommandQueue)))) {
			return false;
		}

		if (FAILED(m_poDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_poDirectCmdListAlloc)))) {
			return false;
		}

		if (FAILED(m_poDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_poDirectCmdListAlloc, // Associated command allocator
			nullptr,                // Initial PipelineStateObject
			IID_PPV_ARGS(&m_poCommandList)))) {
			return false;
		}

		// Start off in a closed state.
		m_poCommandList->Close();

		return true;
	}

	bool GraphicsHandler::CreateSwapChain() {
		// Release the previous swapchain we will be recreating.
		if (m_poSwapChain != nullptr)
			m_poSwapChain->Release();

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_poAve->GetWindowWidth();
		sd.BufferDesc.Height = m_poAve->GetWindowHeight();
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = m_eBackBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = m_b4xMsaaState ? 4 : 1;
		sd.SampleDesc.Quality = m_b4xMsaaState ? (m_i4xMsaaQuality - 1) : 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
		sd.OutputWindow = m_poAve->GetWindow();
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Note: Swap chain uses queue to perform flush.
		HRESULT res = m_poFactory->CreateSwapChain(
			m_poCommandQueue,
			&sd,
			&m_poSwapChain);

		return true;
	}

	bool GraphicsHandler::CreateRtv_Dsv_CBVDescriptorHeaps() {
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		if (FAILED(m_poDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(&m_poRtvHeap)))) {
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		if (FAILED(m_poDevice->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(&m_poDsvHeap)))) {
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = 1000;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		if (FAILED(m_poDevice->CreateDescriptorHeap(
			&cbvHeapDesc, IID_PPV_ARGS(&m_poCbvHeap)))) {
			return false;
		}

		return true;
	}

#pragma endregion

#pragma region Generic Render Composite Methods

	void GraphicsHandler::RenderBegin() {
		ResetCommandList();
		TransitionFromPresentToRenderTarget();
		ClearRenderTargetAndDepthStencil();
		SetCbvDescriptor();
	}

	void GraphicsHandler::RenderCease() {
		TransitionFromRenderTargetToPresent();
		CloseCommandList();
		QueueCommandList();
		Present();
		FlushCommandQueue();
	}

#pragma endregion

#pragma region Rendering

	void GraphicsHandler::ResetCommandList() {
		m_poCommandList->Reset(m_poDirectCmdListAlloc, nullptr);
		m_poCommandList->RSSetViewports(1, &m_oScreenViewport);
		m_poCommandList->RSSetScissorRects(1, &m_oScissorRect);
	}

	void GraphicsHandler::TransitionFromPresentToRenderTarget() {
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_poCommandList->ResourceBarrier(1, &transition);
	}

	void GraphicsHandler::TransitionFromRenderTargetToPresent() {
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);
		m_poCommandList->ResourceBarrier(1, &transition);
	}

	void GraphicsHandler::ClearRenderTargetAndDepthStencil() {
		m_poCommandList->ClearRenderTargetView(CurrentBackBufferView(), m_cFillColor, 0, nullptr);
		m_poCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		auto currentBuffer = CurrentBackBufferView();
		auto depthStencil = DepthStencilView();
		m_poCommandList->OMSetRenderTargets(1, &currentBuffer, true, &depthStencil);
	}

	void GraphicsHandler::SetCbvDescriptor() {
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_poCbvHeap };
		m_poCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	}
		

	void GraphicsHandler::CloseCommandList() {
		if (FAILED(m_poCommandList->Close())) {
			return;
		}
	}

	void GraphicsHandler::QueueCommandList() {
		ID3D12CommandList* cmdsLists[] = { GraphicsHandler::m_poCommandList };
		m_poCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	}

	void GraphicsHandler::Present() {
		if (FAILED(m_poSwapChain->Present(0, 0))) {
			return;
		}
		m_iCurrBackBuffer = (m_iCurrBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
	}

	void GraphicsHandler::FlushCommandQueue() {
		m_iCurrentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		if (FAILED(m_poCommandQueue->Signal(m_poFence, m_iCurrentFence))) {
			return;
		}

		// Wait until the GPU has completed commands up to this fence point.
		if (m_poFence->GetCompletedValue() < m_iCurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, L"false", false, EVENT_ALL_ACCESS);

			// Fire event when GPU hits current fence.  
			if (FAILED(m_poFence->SetEventOnCompletion(m_iCurrentFence, eventHandle))) {
				return;
			}

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

#pragma endregion

#pragma region Getters

	ID3D12Resource* GraphicsHandler::CurrentBackBuffer() const {
		return m_prSwapChainBuffer[m_iCurrBackBuffer];
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GraphicsHandler::CurrentBackBufferView() const {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_poRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_iCurrBackBuffer,
			m_iRtvDescriptorSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GraphicsHandler::DepthStencilView() const {
		return m_poDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

#pragma endregion

	void GraphicsHandler::Release() {
		delete this;
	}
}