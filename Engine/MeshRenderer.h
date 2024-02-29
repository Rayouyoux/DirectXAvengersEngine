#pragma once

namespace ave {
	class Mesh;
	class Shader;
	class MeshRenderer
	{
	public:

		MeshRenderer();

		void Init(ID3D12GraphicsCommandList* pList, ID3D12Device* poDevice);
		void Draw(ID3D12GraphicsCommandList* pList, ID3D12Device* poDevice);
		void Destroy();
		~MeshRenderer();

	protected:
		Mesh* m_poMesh;
		Shader* m_poShader;
	};
}

