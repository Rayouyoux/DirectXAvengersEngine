#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <map>

namespace ave {

	class GraphicsHandler;

	class Texture {


	protected:

		ID3D12Device* m_poDevice;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_poUploadHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_poRessource;


		CD3DX12_CPU_DESCRIPTOR_HANDLE m_poDescriptorCPU;

		CD3DX12_GPU_DESCRIPTOR_HANDLE m_poDescriptorGPU;

		D3D12_SHADER_RESOURCE_VIEW_DESC m_poSrvDesc;

		UINT m_oCbvSrvDescriptorSize;

		ID3D12DescriptorHeap* m_poSrvDescriptorHeap;


	public:
		Texture();
		~Texture();

		void Init(ID3D12Device* poDevice);

		void LoadTexture(std::string oName, std::wstring oFilename, ID3D12DescriptorHeap* CbvDescriptorHeap, GraphicsHandler* oGraphics);
		bool BuildSrvDesc(ID3D12DescriptorHeap* CbvDescriptorHeap, int size);

		ID3D12DescriptorHeap* GetDescriptorHeap() { return m_poSrvDescriptorHeap; };
		UINT* GetDescriptorSize() { return &m_oCbvSrvDescriptorSize; };
		CD3DX12_CPU_DESCRIPTOR_HANDLE* GetDescriptorHandle() { return &m_poDescriptorCPU; };
		CD3DX12_GPU_DESCRIPTOR_HANDLE* GetDescriptorGpuHandle() { return &m_poDescriptorGPU; };
		
	};

}

