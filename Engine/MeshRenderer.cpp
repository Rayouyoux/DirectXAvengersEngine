#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include <d3d12.h>
#include "Mesh.h"
#include "D3DUtils.h"
#include "Shader.h"
namespace ave {
	MeshRenderer::MeshRenderer() {
		
	}

	void MeshRenderer::Init(ID3D12GraphicsCommandList* poList, ID3D12Device* poDevice) {

		m_poShader = new Shader();
		m_poShader->Start(poList, poDevice);

	}
	void MeshRenderer::Draw(ID3D12GraphicsCommandList* poList, ID3D12Device* poDevice) {
		//m_poShader->Create((BYTE*)L"shader.hlsl", sizeof(BYTE*));
		auto oVertexBufferView = m_poMesh->VertexBufferView();
		auto oIndexBufferView = m_poMesh->IndexBufferView();
		poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
		poList->IASetIndexBuffer(&oIndexBufferView);
		poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootObject(), m_poShader->GetVirtualAdress());
		poList->DrawIndexedInstanced(m_poMesh->GetIndexCount(),1,0,0,0);
	}

	void MeshRenderer::Destroy() {
		delete m_poShader;
	}

	MeshRenderer::~MeshRenderer() {
		
	}


}