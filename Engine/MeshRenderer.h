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
		void SetFirstTexture(Texture* texture);

		void Start() override;
		void Update(float dT) override;
		void LateUpdate(float dT) override {};
		void Render() override;
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		Shader* m_poShader;
		Texture* m_poTexture;
		UploadBuffer<ObjectConstants>* m_poBuffer;
	};
}

