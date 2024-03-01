#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include <d3d12.h>
#include "Mesh.h"
#include "D3DUtils.h"
#include "Shader.h"
namespace ave {
	MeshRenderer::MeshRenderer() : Component(){
		m_poMesh = nullptr;
		m_poShader = nullptr;
	}

	void MeshRenderer::SetMesh(Mesh* poMesh) {
		m_poMesh = poMesh;
	}

	void MeshRenderer::SetShader(Shader* poShader) {
		m_poShader = poShader;
	}

	void MeshRenderer::Start() {

	}
	void MeshRenderer::Render(ID3D12GraphicsCommandList* poList, ID3D12Device* poDevice) {
		
		auto oVertexBufferView = m_poMesh->VertexBufferView();
		auto oIndexBufferView = m_poMesh->IndexBufferView();
		poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
		poList->IASetIndexBuffer(&oIndexBufferView);

		poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootObject(), m_poShader->GetVirtualAdress());



		poList->DrawIndexedInstanced(m_poMesh->GetIndexCount(),1,0,0,0);
	}

	void MeshRenderer::Destroy() {

	}

	MeshRenderer::~MeshRenderer() {
		
	}


}
