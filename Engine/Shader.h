#pragma once

#include <d3d12.h>
#include <vector>
#include "UploadBuffer.h"
#include <cstring>
#include <string>
#include "Texture.h"

namespace ave {
	class Mesh;
	class GraphicsHandler;
	class Texture;
	class Shader
	{

	protected:

		GraphicsHandler* m_poGraphics;
		ID3D12Device* m_poDevice;
		ID3D12DescriptorHeap* m_poCbvHeap;
		ID3DBlob* m_poSerializedRootSig;
		UploadBuffer* m_poPass;
		std::vector<UploadBuffer*> m_voObjects;
		ID3D12RootSignature* m_poRootSignature;
		ID3DBlob* m_poVS;
		ID3DBlob* m_poPS;
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_oInputLayout;
		ID3D12PipelineState* m_poPso;


		int m_iIdRootSignature;
		int m_iRootTexture;
		int m_iRootTexture2;
		int m_iRootObject;
		int m_iRootPass;
		int m_iTextureCount;

	public:
		Shader();

		void Destroy();
		void Reset();
		//void Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2);
		void End();
		void AddObject();
		bool CreateShader();
		void UpdateObject();
		bool CreateRootSignature(int id);
		void CreateUploadBuffer();

		ID3DBlob* CompileShader(const std::wstring& oBuffer, const std::string& oEntryPoint, const std::string& oTarget);
		

		ID3D12RootSignature* GetRootSignature();
		UINT GetRootObject();
		D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAdress();
		ID3D12PipelineState* GetPso();
		UploadBuffer* GetPass();

		~Shader();

	};
}


