#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

#pragma region Direct3D Forward Declarations
class IDXGIFactory4;
class IDXGISwapChain;
#pragma endregion

namespace ave {
	class AvengersEngine;

	class GraphicsHandler {
	protected:
		AvengersEngine* m_poAve;

		bool m_b4xMsaaState;
		UINT m_i4xMsaaQuality;

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

		DirectX::XMVECTORF32 m_cFillColor;
		D3D_DRIVER_TYPE m_eDriverType;
		DXGI_FORMAT m_eBackBufferFormat;
		DXGI_FORMAT m_eDepthStencilFormat;

	protected:
		GraphicsHandler();
		GraphicsHandler(const GraphicsHandler& oGraphics) = delete;
		GraphicsHandler& operator=(const GraphicsHandler& graphics) = delete;
		~GraphicsHandler();

	protected:
		virtual bool CreateFactory();
		virtual bool CreateDevice();
		virtual bool CreateFence();
		virtual bool RequestMsaaQuality();
		virtual bool CreateCommandObjects();
		virtual bool CreateSwapChain();
		virtual bool CreateRtvAndDsvDescriptorHeaps();

		/// <summary>
		/// Generic initial steps of the Render method.
		/// </summary>
		virtual void RenderBegin();
		/// <summary>
		/// Generic close steps of the Render method.
		/// </summary>
		virtual void RenderCease();
		/// <summary>
		/// Reset the command list. Must be called at the begining of Rendering.
		/// </summary>
		virtual void ResetCommandList();
		/// <summary>
		/// Tells the back buffer to go from Present to Render State.
		/// </summary>
		virtual void TransitionFromPresentToRenderTarget();
		/// <summary>
		/// Tells the back buffer to go from Render State to Present.
		/// </summary>
		virtual void TransitionFromRenderTargetToPresent();
		/// <summary>
		/// Clear the render target with m_cFillColor and the Depth Stencil.
		/// </summary>
		/// <param name="cfillColor">White is the default color</param>
		virtual void ClearRenderTargetAndDepthStencil();
		/// <summary>
		/// Close the command list.
		/// </summary>
		virtual void CloseCommandList();
		/// <summary>
		/// Queue the command list, must be called when it is in closed state.
		/// </summary>
		virtual void QueueCommandList();
		/// <summary>
		/// Present the current backbuffer as the front buffer.
		/// </summary>
		virtual void Present();
		/// <summary>
		/// Clear the GPU Command Queue.
		/// </summary>
		virtual void FlushCommandQueue();

		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

	public:
		static GraphicsHandler* Create();
		virtual bool Initialize(AvengersEngine* poAve);

		virtual void OnResize();

		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		// Set & Getters
		virtual DirectX::XMVECTORF32 GetFillColor() const { return m_cFillColor; }
		virtual void SetFillColor(DirectX::XMVECTORF32& cfillColor) { m_cFillColor = cfillColor; }

		virtual ID3D12Device* GetDevice() const { return m_poDevice; }

		bool Get4xMsaaState() const { return m_b4xMsaaState; }
		UINT Get4xMsaaQuality() const { return m_i4xMsaaQuality; }
		void Set4xMsaaState(bool value) { m_b4xMsaaState = value; }

		virtual void Release();
	};
}