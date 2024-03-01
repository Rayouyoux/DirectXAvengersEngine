#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include <d3d12.h>
#include "Mesh.h"
#include "D3DUtils.h"
#include "Shader.h"
#include "GraphicsHandler.h"
#include "UploadBuffer.h"

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

	
	void MeshRenderer::Render() {
		//Root
		ID3D12GraphicsCommandList* poList = GraphicsHandler::GetCommandList();

		poList->SetGraphicsRootSignature(m_poShader->GetRootSignature());

		//Pass
		poList->SetGraphicsRootConstantBufferView(1, m_poShader->GetPass()->Resource()->GetGPUVirtualAddress());

		//Create((BYTE*)L"shader.hlsl", sizeof(BYTE*));

		//Pipeline
		poList->SetPipelineState(m_poShader->GetPso());

		//Topology
		poList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ID3D12GraphicsCommandList* poList = GraphicsHandler::GetCommandList();
		auto oVertexBufferView = m_poMesh->VertexBufferView();
		auto oIndexBufferView = m_poMesh->IndexBufferView();
		poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
		poList->IASetIndexBuffer(&oIndexBufferView);

		poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootObject(), m_poShader->GetVirtualAdress());

		//Shader

		poList->DrawIndexedInstanced(m_poMesh->GetIndexCount(),1,0,0,0);
	}

	void MeshRenderer::Destroy() {

	}

	MeshRenderer::~MeshRenderer() {
		
	}


}
