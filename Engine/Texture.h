#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <map>

namespace ave {

	class Texture {


	protected:
		std::string m_oName;

		std::map<std::string, Texture*> m_mTextures;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_poRessource;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_poUploadHeap;

		CD3DX12_CPU_DESCRIPTOR_HANDLE m_pohDescriptor;

		D3D12_SHADER_RESOURCE_VIEW_DESC m_poSrvDesc;

		UINT m_oCbvSrvDescriptorSize;

		ID3D12DescriptorHeap* m_poSrvDescriptorHeap;


	public:
		Texture();
		~Texture();

		void Init();

		void LoadTexture(std::string oName, std::wstring oFilename);
		bool BuildDescriptorHeaps(std::string oName);
		void Offset(std::string oName);

		ID3D12DescriptorHeap* GetDescriptorHeap() { return m_poSrvDescriptorHeap; };
		UINT GetDescriptorSize() { return m_oCbvSrvDescriptorSize; };
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() { return m_pohDescriptor; };

	};

}

