#pragma once

#include <d3d12.h>
#include <vector>
#include <cstring>
#include <string>
#include "UploadBuffer.h"
#include "Texture.h"
#include "ConstantsStruct.h"

namespace ave {
	class Mesh;
	class GraphicsHandler;
	class Texture;
	class Camera;
	
	class Shader
	{

	protected:

		GraphicsHandler* m_poGraphics;
		ID3D12Device* m_poDevice;
		ID3D12DescriptorHeap* m_poCbvHeap;
		ID3DBlob* m_poSerializedRootSig;
		UploadBuffer<PassConstants>* m_poPass;
		UploadBuffer<ObjectConstants>* m_poObject;
		std::vector<UploadBuffer<ObjectConstants>*> m_voObjects;
		ID3D12RootSignature* m_poRootSignature;
		ID3DBlob* m_poVS;
		ID3DBlob* m_poPS;
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_oInputLayout;
		ID3D12PipelineState* m_poPso;
		Camera* m_poCamera;


		int m_iIdRootSignature;
		int m_iRootTexture;
		int m_iRootTexture2;
		int m_iRootObject;
		int m_iRootPass;
		int m_iTextureCount;
		int m_iIndexObject;

	public:
		Shader();

		void Destroy();
		/*void ResetIndexObject();*/
		//void Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2);
		void End();
		/*void AddObject();*/
		bool CreateShader(GraphicsHandler* poGraphicsHandler, Camera* poCamera);
		void Draw(Mesh* pMesh, UploadBuffer<ObjectConstants>* poBuffer);
		/*void UpdateObject();*/
		bool CreateRootSignature(int id);

		ID3DBlob* CompileShader(const std::wstring& oBuffer, const std::string& oEntryPoint, const std::string& oTarget);
		
		ID3D12Device* GetDevice();
		ID3D12RootSignature* GetRootSignature();
		UINT GetRootObject();
		UINT GetRootPass();
		ID3D12PipelineState* GetPso();


		~Shader();

	};
}


