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

		////Pass
		poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootPass(), m_poShader->GetPass()->Resource()->GetGPUVirtualAddress());


		////Pipeline
		poList->SetPipelineState(m_poShader->GetPso());

		////Topology
		poList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//

   //     if (m_poShader->GetRootTexture() != -1 && pTexture)
			//poList->SetGraphicsRootDescriptorTable(m_poShader->GetRootTexture(), );

   //     if (m_poShader->GetRootTexture() != -1 && pTexture2)
			//poList->SetGraphicsRootDescriptorTable(m_poShader->GetRootTexture(), /*D3D12_GPU_DESCRIPTOR_HANDLE de la classe pTexture */);

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
