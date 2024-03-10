#pragma once
#include <d3d12.h>
#include "Component.h"
#include "UploadBuffer.h"

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
		void Destroy();
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		UploadBuffer<ObjectConstants>* m_poBuffer;
		Texture* m_poTexture;
		Shader* m_poShader;
	};
}

