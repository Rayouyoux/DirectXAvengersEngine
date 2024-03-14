#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <windows.h>

#pragma region Direct3D Forward Declarations
class IDXGIFactory4;
class IDXGISwapChain;
#pragma endregion

namespace ave {
	class AvengersEngine;
	class Shader;
	class MeshRenderer;
	class Entity;
	class Camera;

	namespace Particles {
		class ParticleSystem;
		class ParticleBehaviour;
	}
	
	class EntityManager;
	
	class Texture;
	
	class Mesh;

	class GraphicsHandler {
	protected:
		AvengersEngine* m_poAve;

		Shader* m_poShader;
		Mesh* m_poMesh;
		
		Camera* m_poCamera;
		Particles::ParticleBehaviour* m_poBehaviour;
		Particles::ParticleSystem* m_poParticleSystem;
		Texture* m_poTexture;

		bool m_b4xMsaaState;
		UINT m_i4xMsaaQuality;

		IDXGIFactory4* m_poFactory;
		IDXGISwapChain* m_poSwapChain;
		static ID3D12Device* m_poDevice;

		ID3D12Fence* m_poFence;
		UINT64 m_iCurrentFence;

		ID3D12CommandQueue* m_poCommandQueue;
		ID3D12CommandAllocator* m_poDirectCmdListAlloc;
		static ID3D12GraphicsCommandList* m_poCommandList;

		static const int SWAP_CHAIN_BUFFER_COUNT = 2;
		int m_iCurrBackBuffer;
		ID3D12Resource* m_prSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
		ID3D12Resource* m_prDepthStencilBuffer;

		ID3D12DescriptorHeap* m_poRtvHeap;
		ID3D12DescriptorHeap* m_poDsvHeap;
		ID3D12DescriptorHeap* m_poCbvHeap;

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
		virtual bool CreateRtv_Dsv_CBVDescriptorHeaps();

		 //<summary>
		 //Generic initial steps of the Render method.
		 //</summary>
		virtual void RenderBegin();
		/// <summary>
		/// Generic close steps of the Render method.
		/// </summary>
		virtual void RenderCease();
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
		/// Set the Cbv descriptor with m_poCbvHeap.
		/// </summary>
		virtual void SetCbvDescriptor();


		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

	public:
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
		/// Reset the command list. Must be called at the begining of Rendering.
		/// </summary>
		virtual void ResetCommandList();
		/// <summary>
		/// Clear the GPU Command Queue.
		/// </summary>
		virtual void FlushCommandQueue();

		//a voir ???
		EntityManager* m_poEntityManager;

		static GraphicsHandler* Create();
		virtual bool Initialize(AvengersEngine* poAve);

		virtual void OnResize();

		virtual void Update(float deltaTime);
		virtual void LateUpdate();
		virtual void Render();

		// Set & Getters
		virtual DirectX::XMVECTORF32 GetFillColor() const { return m_cFillColor; }
		virtual void SetFillColor(const DirectX::XMVECTORF32& cfillColor) { m_cFillColor = cfillColor; }

		static ID3D12Device* GetDevice() { return m_poDevice; }
		static ID3D12GraphicsCommandList* GetCommandList() { return GraphicsHandler::m_poCommandList; };
		ID3D12CommandQueue* GetCommandQueue() { return m_poCommandQueue; };
		ID3D12CommandAllocator* GetCommandAlloc() { return m_poDirectCmdListAlloc; };

		ID3D12DescriptorHeap* GetCbvDescriptor() { return m_poCbvHeap; };
		HWND GetWindow();

		bool Get4xMsaaState() const { return m_b4xMsaaState; }
		UINT Get4xMsaaQuality() const { return m_i4xMsaaQuality; }
		void Set4xMsaaState(bool value) { m_b4xMsaaState = value; }

		virtual void Release();
	};
}