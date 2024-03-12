#pragma once
#include <d3d12.h>
#include "Component.h"
#include "UploadBuffer.h"
#include <string>

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

		void SetName(std::string oName);

		void Start() override;
		void Update(float dT) override;
		void LateUpdate(float dT) override {};
		void Render() override;
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		UploadBuffer<ObjectConstants>* m_poBuffer;
		std::string m_oName;
		Shader* m_poShader;
	};
}

