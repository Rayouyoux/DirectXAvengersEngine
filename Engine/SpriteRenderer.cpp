#include "pch.h"
#include "SpriteRenderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "GraphicsHandler.h"
#include "Texture.h"

namespace ave {
	namespace UI {
		SpriteRenderer::SpriteRenderer() {
			m_poPlane = nullptr; // GetMesh from Manager
			m_poShader = nullptr;
		}

		SpriteRenderer::~SpriteRenderer()
		{}

		void SpriteRenderer::Start() {
			XMFLOAT2 arbSize = { 1600, 900 };

			XMVECTOR posScale = XMVectorSet(
				m_vPositionPx.x / arbSize.x,
				m_vPositionPx.y / arbSize.y,
				0, 0
			);

			XMVECTOR sizeScale = XMVectorSet(
				m_vSizePx.x / arbSize.x,
				m_vSizePx.y / arbSize.y,
				0, 0
			);

			m_poEntity->m_poTransform->SetVectorPosition(&posScale);
			m_poEntity->m_poTransform->SetVectorScale(&sizeScale);

			m_poBuffer = new UploadBuffer<ObjectConstants>(GraphicsHandler::GetDevice(), 1, true);
		}

		void SpriteRenderer::Update(float dT) {
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
			m_poBuffer->CopyData(0, objConstants);
		}

		void SpriteRenderer::Render2D() {
			m_poShader->Draw(m_poPlane, m_poBuffer, m_poSprite);
		}

		int SpriteRenderer::GetComponentID() {
			return SPRITE_RENDERER_ID;
		}
	}
}