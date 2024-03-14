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
#include "Maths.h"

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
	void MeshRenderer::SetFirstTexture(Texture* poTexture) {
		m_poTexture = poTexture;
	}

	void MeshRenderer::Start() {
		SetAABB(*m_poMesh->GetAABB());
	}

	void MeshRenderer::Update(float deltaTime) {
		if (m_poEntity->m_poTransform->m_bMoved) {
			UpdateAABBAfterTranslation(m_poEntity->m_poTransform->GetVectorPosition());
			m_poEntity->m_poTransform->m_bMoved = false;
		}
		if (m_poEntity->m_poTransform->m_bRotated) {
			UpdateAABBAfterRotation(m_poEntity->m_poTransform->GetMatrixRotation());
			m_poEntity->m_poTransform->m_bRotated = false;
		}
		if (m_poEntity->m_poTransform->m_bScaled) {
			UpdateAABBAfterScale(m_poEntity->m_poTransform->GetVectorScale());
			m_poEntity->m_poTransform->m_bScaled = false;
		}
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
		m_poBuffer->CopyData(0, objConstants);
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

		m_poShader->Draw(m_poMesh, m_poBuffer);

	}

	void MeshRenderer::UpdateAABBAfterRotation(const XMMATRIX& vRotationMatrix) {
		
		XMFLOAT4X4 rotationMatrix;
		XMStoreFloat4x4(&rotationMatrix, vRotationMatrix);
		
		for (auto& vertex : m_poMesh->GetVertices()) {
			XMVECTOR transformedVertex = XMVector3Transform(XMLoadFloat3(&vertex), XMLoadFloat4x4(&rotationMatrix));
			XMFLOAT3 transformedVertexFloat3;
			XMStoreFloat3(&transformedVertexFloat3, transformedVertex);
			UpdateAABB(transformedVertexFloat3);
		}
	}

	void MeshRenderer::UpdateAABBAfterTranslation(const XMVECTOR& vTranslation) {

		XMFLOAT3 translation;
		XMStoreFloat3(&translation, vTranslation);

		AABB oAABB = GetAABB();

		oAABB.m_vMin.x += translation.x;
		oAABB.m_vMin.y += translation.y;
		oAABB.m_vMin.z += translation.z;

		oAABB.m_vMax.x += translation.x;
		oAABB.m_vMax.y += translation.y;
		oAABB.m_vMax.z += translation.z;

		SetAABB(oAABB);
	}

	void MeshRenderer::UpdateAABBAfterScale(const XMVECTOR& vScale) {

		XMFLOAT3 scale;
		XMStoreFloat3(&scale, vScale);

		AABB oAABB = GetAABB();

		oAABB.m_vMin.x *= scale.x;
		oAABB.m_vMin.y *= scale.y;
		oAABB.m_vMin.z *= scale.z;

		oAABB.m_vMax.x *= scale.x;
		oAABB.m_vMax.y *= scale.y;
		oAABB.m_vMax.z *= scale.z;

		SetAABB(oAABB);
	}

	void MeshRenderer::UpdateAABB(XMFLOAT3& vertex) {

		AABB oAABB = GetAABB();

		oAABB.m_vMin.x = Maths::TakeLowest(oAABB.m_vMin.x, vertex.x);
		oAABB.m_vMin.y = Maths::TakeLowest(oAABB.m_vMin.y, vertex.y);
		oAABB.m_vMin.z = Maths::TakeLowest(oAABB.m_vMin.z, vertex.z);

		oAABB.m_vMax.x = Maths::TakeHighest(oAABB.m_vMax.x, vertex.x);
		oAABB.m_vMax.y = Maths::TakeHighest(oAABB.m_vMax.y, vertex.y);
		oAABB.m_vMax.z = Maths::TakeHighest(oAABB.m_vMax.z, vertex.z);
	}

	Mesh* MeshRenderer::GetMesh() {
		return m_poMesh;
	}

	AABB MeshRenderer::GetAABB()
	{
		return m_oContainingBox;
	}

	void MeshRenderer::SetAABB(AABB oAABB)
	{
		m_oContainingBox = oAABB;
	}

	MeshRenderer::~MeshRenderer() {
		delete m_poBuffer;
	}
}
