#include "pch.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "GraphicsHandler.h"
//#include <d3d12.h>

namespace ave {

	Texture::Texture() {
		m_poSrvDesc = {};

		m_poDevice = nullptr;
		m_poUploadHeap = nullptr;
		m_poSrvDescriptorHeap = nullptr;
		m_oCbvSrvDescriptorSize = 0;
	}

	void Texture::Init(ID3D12Device* poDevice) {
		
		m_poDevice = poDevice;

		m_oCbvSrvDescriptorSize = m_poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void Texture::LoadTexture(std::string oName, std::wstring oFilename, ID3D12DescriptorHeap* CbvDescriptorHeap) {

		if (FAILED(DirectX::CreateDDSTextureFromFile12(m_poDevice,
			GraphicsHandler::GetCommandList(),oFilename.c_str(),
			m_poRessource, m_poUploadHeap))) {
			return;
		}
	}

	bool Texture::BuildSrvDesc(ID3D12DescriptorHeap* CbvDescriptorHeap, std::map<std::string, Texture*> voTextures) {

		m_poSrvDescriptorHeap = CbvDescriptorHeap;

		m_poDescriptorCPU = m_poSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		m_poDescriptorCPU.Offset(voTextures.size()-1, m_oCbvSrvDescriptorSize);

		m_poSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		m_poSrvDesc.Format = m_poRessource->GetDesc().Format;
		m_poSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		m_poSrvDesc.Texture2D.MostDetailedMip = 0;
		m_poSrvDesc.Texture2D.MipLevels = m_poRessource->GetDesc().MipLevels;
		m_poSrvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		m_poDevice->CreateShaderResourceView(m_poRessource.Get(), &m_poSrvDesc, m_poDescriptorCPU);
		
		m_poDescriptorGPU = m_poSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
		m_poDescriptorGPU.Offset(voTextures.size() - 1, m_oCbvSrvDescriptorSize);
		
		return true;
	}

	/*void Texture::Offset(std::string oName) {

		auto offset = m_mTextures[oName];

		m_pohDescriptor.Offset(m_mTextures.size()-1, m_oCbvSrvDescriptorSize);
		m_poSrvDesc.Format = offset->GetDesc().Format;
		m_poSrvDesc.Texture2D.MipLevels = offset->GetDesc().MipLevels;

		m_poDevice->CreateShaderResourceView(offset.Get(), &m_poSrvDesc, m_pohDescriptor);

	}*/

	Texture::~Texture() {
		m_mTextures.clear();
		m_poDevice->Release();
		m_poSrvDescriptorHeap->Release();
	}
}