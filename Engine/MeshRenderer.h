#pragma once
#include <d3d12.h>
#include "Component.h"
#include "UploadBuffer.h"
#include <DirectXMath.h>
#include "AABB.h"

using namespace DirectX;

namespace ave {
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

		void SetFirstTexture(Texture* poTexture);

		void Start() override;
		void Update(float dT) override;
		void LateUpdate(float dT) override {};
		void Render() override;
		
		void UpdateAABBAfterRotation(const XMMATRIX& vRotationMatrix);
		void UpdateAABBAfterTranslation(const XMVECTOR& vTranslation);
		void UpdateAABBAfterScale(const XMVECTOR& vScale);
		void UpdateAABB(XMFLOAT3& vertex);

		Mesh* GetMesh();
		AABB GetAABB();
		void SetAABB(AABB oAABB);

		~MeshRenderer();


	protected:
		Mesh* m_poMesh;
		AABB m_oContainingBox;
		UploadBuffer<ObjectConstants>* m_poBuffer;
		Texture* m_poTexture;
		Shader* m_poShader;
	};
}

