#pragma once
#include <d3d12.h>
#include "Component.h"
#include "UploadBuffer.h"
#include <string>

namespace ave {
	struct AABB{};
	class Mesh;
	class Shader;
	struct ObjectConstants;
	class Texture;

	class MeshRenderer : public Component
	{
	public:

		MeshRenderer();
	
		void SetMesh(Mesh* poMesh);
		void SetShader(Shader* SetShader);

		void SetTexture(Texture* oTexture);
		void SetColor(DirectX::FXMVECTOR* vColor);

		void Start() override;
		void Update(float dT) override;
		void LateUpdate(float dT) override {};
		void Render() override;
		~MeshRenderer();

		AABB GetAABB();
		void SetAABB(AABB oAABB);

	protected:
		Mesh* m_poMesh;
		Texture* m_poTexture;
		UploadBuffer<ObjectConstants>* m_poBuffer;
		std::string m_oName;
		Shader* m_poShader;

		DirectX::XMFLOAT4 m_oColor;

		AABB m_oContainingBox;

		void UpdateAABBAfterRotation(const XMMATRIX& vRotationMatrix);
		void UpdateAABBAfterTranslation(const XMVECTOR& vTranslation);
		void UpdateAABBAfterScale(const XMVECTOR& vScale);
		void UpdateAABB(XMFLOAT3& vertex);
	};
}

