#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include <d3d12.h>
#include "Mesh.h"
#include "D3DUtils.h"
#include "Shader.h"
#include "GraphicsHandler.h"
#include "UploadBuffer.h"
#include "ConstantsStruct.h"
#include "Entity.h"
#include "Transform.h"
#include "Texture.h"

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
		m_poBuffer = new UploadBuffer<ObjectConstants>(m_poShader->GetDevice(), 1, true);
	}
	void MeshRenderer::SetTexture(Texture* oTexture) {
		m_poTexture = oTexture;
	}

	void MeshRenderer::Start() {
		
	}

	void MeshRenderer::Update(float deltaTime) {
		XMVECTOR color = XMVectorSet(0.5f, 0.3f, 0.2f, 1.0f);
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
		//objConstants.Color = SetColor(&color);
		m_poBuffer->CopyData(0, objConstants);
	};

	DirectX::XMFLOAT4 MeshRenderer::SetColor(DirectX::FXMVECTOR* vColor) {
		XMFLOAT4 color;
		if (vColor != nullptr) {
			XMStoreFloat4(&color, *vColor);
			return color;
		};
	}

	void MeshRenderer::Render() {
		//ID3D12GraphicsCommandList* poList = GraphicsHandler::GetCommandList();

		////Root
		//poList->SetGraphicsRootSignature(m_poShader->GetRootSignature());

		//poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootPass(), m_poShader->GetPass()->Resource()->GetGPUVirtualAddress());

		//////Create((BYTE*)L"shader.hlsl", sizeof(BYTE*));

		//////Pipeline
		//poList->SetPipelineState(m_poShader->GetPso());

		//////Topology
		//poList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//auto oVertexBufferView = m_poMesh->VertexBufferView();
		//auto oIndexBufferView = m_poMesh->IndexBufferView();
		//poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
		//poList->IASetIndexBuffer(&oIndexBufferView);

		//poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootObject(), m_poBuffer->Resource()->GetGPUVirtualAddress());

 
  //      if (m_poShader->GetRootTexture() != -1 && m_poTexture)
		//{ 
		//	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_poTexture->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

		////Pipeline
		/*poList->SetPipelineState(m_poShader->GetPso("transparent"));*/

		//}

		//auto oVertexBufferView = m_poMesh->VertexBufferView();
		//auto oIndexBufferView = m_poMesh->IndexBufferView();
		//poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
		//poList->IASetIndexBuffer(&oIndexBufferView);
		

		m_poShader->Draw(m_poMesh, m_poBuffer, m_poTexture);

	}

	MeshRenderer::~MeshRenderer() {
		delete m_poBuffer;
	}
}
