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
	void MeshRenderer::SetFirstTexture(Texture* poTexture) {
		m_poTexture = poTexture;
	}

	void MeshRenderer::Start() {
		
	}

	void MeshRenderer::Update(float deltaTime) {
		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
		m_poBuffer->CopyData(0, objConstants);
	}

	void MeshRenderer::Render() {
		m_poShader->Draw(m_poMesh, m_poBuffer);
	}

	MeshRenderer::~MeshRenderer() {
		delete m_poBuffer;
	}
}
