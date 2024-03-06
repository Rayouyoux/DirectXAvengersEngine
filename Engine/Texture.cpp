#include "pch.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "GraphicsHandler.h"
//#include <d3d12.h>

namespace ave {

	Texture::Texture() {

		m_poUploadHeap = nullptr;
		m_poSrvDescriptorHeap = nullptr;
		m_oCbvSrvDescriptorSize = 0;
	}
	Texture::~Texture() {

	}

	void Texture::Init(ID3D12Device* poDevice, ID3D12DescriptorHeap* heap) {
		
		m_poDevice = poDevice;

		m_poSrvDescriptorHeap = heap;
		m_oCbvSrvDescriptorSize = poDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void Texture::LoadTexture(std::string oName, std::wstring oFilename) {

		m_oName = oName;

		if (FAILED(DirectX::CreateDDSTextureFromFile12(m_poDevice,
			GraphicsHandler::GetCommandList(),oFilename.c_str(),
			m_poRessource,m_poUploadHeap))) {
			return;
		}

		m_mTextures[m_oName] = std::move(this);

	}

	bool Texture::BuildDescriptorHeaps(std::string oName) {

		/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 100;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (FAILED(m_poDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_poSrvDescriptorHeap)))) {
			return false;
		};*/

		m_pohDescriptor = m_poSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		auto texture = m_mTextures[oName]->m_poRessource;

		m_poSrvDesc = {};
		m_poSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		m_poSrvDesc.Format = texture->GetDesc().Format;
		m_poSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		m_poSrvDesc.Texture2D.MostDetailedMip = 0;
		m_poSrvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		m_poSrvDesc.Texture2D.ResourceMinLODClamp = 0.0f;



		m_poDevice->CreateShaderResourceView(texture.Get(), &m_poSrvDesc, m_pohDescriptor);
		
		return true;
	}

	void Texture::Offset(std::string oName) {

		auto offset = m_mTextures[oName]->m_poRessource;

		m_pohDescriptor.Offset(1, m_oCbvSrvDescriptorSize);
		m_poSrvDesc.Format = offset->GetDesc().Format;
		m_poSrvDesc.Texture2D.MipLevels = offset->GetDesc().MipLevels;

		m_poDevice->CreateShaderResourceView(offset.Get(), &m_poSrvDesc, m_pohDescriptor);
	}
}