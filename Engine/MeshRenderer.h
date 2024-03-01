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

		void Start() override;
		void Render(ID3D12GraphicsCommandList* poList, ID3D12Device* poDevice);
		void Destroy();
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		Shader* m_poShader;

	};
}

