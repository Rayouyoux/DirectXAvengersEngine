#pragma once

#include <d3d12.h>
#include <vector>
#include "UploadBuffer.h"
#include <cstring>
#include <string>
#include "Texture.h"

namespace ave {
	class Mesh;
	class D3DApp;
	class Texture;
	class Shader
	{
	public:
		Shader();

		ID3D12PipelineState* GetPso();
		void Destroy();
		void Reset();
		void Start(ID3D12GraphicsCommandList* pList, ID3D12Device* poDevice);
		void Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2);
		void End();
		void AddObject();
		bool Create(BYTE* oSrc, int iSize );
		void UpdateObject();
		~Shader();

	protected:
		bool CreateRootSignature(int id);

	public:
		ID3DBlob* Compile(BYTE* oBuffer, int iSize, std::string oEntryPoint, std::string oTarget);

	protected:

		D3DApp* m_poApp;
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
	};
}


