#include <AvengersEngine.h>
#include "d3dUtils.h"
#include "MeshRenderer.h"
#include "Shader.h"

using namespace DirectX;
class AppTest : public ave::D3DApp {
protected:
	ave::MeshRenderer* m_poRenderer;
public:
	ave::Shader* m_poShader;
    AppTest(HINSTANCE hInstance) : D3DApp(hInstance)
    {}

	virtual void Update() override {

	}

	virtual void Render() override {
		
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished execution on the GPU.
		m_poCommandList->Reset(m_poDirectCmdListAlloc, nullptr);


		m_poRenderer = new ave::MeshRenderer();

		m_poRenderer->Init(m_poCommandList,m_poDevice);

		// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		m_poCommandList->RSSetViewports(1, &m_oScreenViewport);
		m_poCommandList->RSSetScissorRects(1, &m_oScissorRect);

		// Indicate a state transition on the resource usage.
		auto transition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_poCommandList->ResourceBarrier(1, &transition);

		// Clear the back buffer and depth buffer.
		m_poCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::RoyalBlue, 0, nullptr);
		m_poCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		auto currentBuffer = CurrentBackBufferView();
		auto depthStencil = DepthStencilView();
		m_poCommandList->OMSetRenderTargets(1, &currentBuffer, true, &depthStencil);

		m_poRenderer->Draw(m_poCommandList, m_poDevice);
		// Indicate a state transition on the resource usage.
		auto transition2 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_poCommandList->ResourceBarrier(1, &transition2);

		// Done recording commands.
		ThrowIfFailed(m_poCommandList->Close());

		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { m_poCommandList };
		m_poCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// swap the back and front buffers
		ThrowIfFailed(m_poSwapChain->Present(0, 0));
		m_iCurrBackBuffer = (m_iCurrBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		FlushCommandQueue();
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {	


        AppTest app(hInstance);
        if (app.Initialize() == false)
            return 0;

        return app.Run();
    }
    catch (ave::DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}