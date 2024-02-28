#pragma once

#include <d3d12.h>
#include <vector>
#include "UploadBuffer.h"
#include <cstring>
#include <string>

namespace ave {
	class Texture;
	class Mesh;

	class Shader
	{
	public:
		Shader();

		void Destroy();
		void Reset();
		void Begin(ID3D12GraphicsCommandList* pList);
		void Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2);
		void End();
		void AddObject();
		bool Create(BYTE* oSrc, int iSize );
		void UpdateObject();


	protected:
		~Shader();
		bool CreateRootSignature(int id);

	public:
		ID3DBlob* Compile(BYTE* oBuffer, int iSize, std::string oEntryPoint, std::string oTarget);

	protected:

		ID3D12Device* m_poDevice;
		ID3D12DescriptorHeap* m_poCbvHeap;
		ID3DBlob* m_poSerializedRootSig;
		UploadBuffer<int>* m_poPass;
		std::vector<UploadBuffer<int>*> m_voObjects;
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
	};
}


