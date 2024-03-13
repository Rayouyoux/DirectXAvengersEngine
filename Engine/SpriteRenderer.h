#pragma once
#include "Component.h"
#include "ConstantsStruct.h"
#include "UploadBuffer.h"

namespace ave {
	class Mesh;
	class Shader;

	namespace UI {
		class SpriteRenderer : public Component {
		protected:
			// Ref to sprite & Setter to render \ aka Texture
			Mesh* m_poPlane;
			Shader* m_poShader;
			UploadBuffer<ObjectConstants>* m_poBuffer;

			DirectX::XMFLOAT2 m_vPositionPx;
			DirectX::XMFLOAT2 m_vSizePx;

		public:
			SpriteRenderer();
			~SpriteRenderer();

			void Start() override;
			void Update(float dT) override;
			void Render2D() override;

			void SetMesh(Mesh* poMesh) {
				m_poPlane = poMesh;
			}

			void SetShader(Shader* poShader) {
				m_poShader = poShader;
			}

			void SetPixelPosition(XMVECTOR& position) {
				XMStoreFloat2(&m_vPositionPx, position);
			}

			void SetPixelSize(XMVECTOR& size) {
				XMStoreFloat2(&m_vSizePx, size);
			}
		};
	}
}