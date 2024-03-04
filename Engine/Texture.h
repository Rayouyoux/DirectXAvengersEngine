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

		ID3D12DescriptorHeap* m_poSrvDescriptorHeap;


	public:
		Texture();
		~Texture();
		void LoadTexture(std::string oName, std::wstring oFilename);
		void BuildDescriptorHeaps(std::string oName);


	};

}

