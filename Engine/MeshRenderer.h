#pragma once
#include "Component.h"
#include <d3d12.h>

namespace ave {
	class Mesh;
	class Shader;

	class MeshRenderer : public Component
	{
	public:

		MeshRenderer();
	
		void SetMesh(Mesh* poMesh);
		void SetShader(Shader* SetShader);

		void Start() override {};
		void Update(float dT) override {};
		void LateUpdate(float dT) override {};
		void Render() override;
		void Destroy();
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		Shader* m_poShader;
	};
}

