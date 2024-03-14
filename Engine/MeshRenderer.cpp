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
		m_oColor = { 0.0f,0.0f,0.0f,0.0f };
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

		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
		objConstants.Color = m_oColor;
		m_poBuffer->CopyData(0, objConstants);
	};

	void MeshRenderer::SetColor(DirectX::FXMVECTOR* vColor) {
		
		if (vColor != nullptr) {
			XMStoreFloat4(&m_oColor, *vColor);
			m_oColor.x /= 255;
			m_oColor.y /= 255;
			m_oColor.z /= 255;
		};
	}

	void MeshRenderer::Render() {
		m_poShader->Draw(m_poMesh, m_poBuffer, m_poTexture);
	}

	MeshRenderer::~MeshRenderer() {
		delete m_poBuffer;
	}
}
