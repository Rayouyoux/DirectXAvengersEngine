#include "pch.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "GraphicsHandler.h"
//#include <d3d12.h>

namespace ave {

	Texture::Texture() {
		m_oName = nullptr;
		m_poRessource = nullptr;
		m_poUploadHeap = nullptr;
		m_poSrvDescriptorHeap = nullptr;
	}
	Texture::~Texture() {

	}
	void Texture::LoadTexture(std::string oName, std::wstring oFilename) {

		m_oName = oName;

		if (FAILED(DirectX::CreateDDSTextureFromFile12(GraphicsHandler::GetDevice(),
			GraphicsHandler::GetCommandList(),oFilename.c_str(),
			m_poRessource,m_poUploadHeap))) {
			return;
		}

		m_mTextures[m_oName] = std::move(this);
	}

	void Texture::BuildDescriptorHeaps(std::string oName) {

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (FAILED(GraphicsHandler::GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_poSrvDescriptorHeap)))) {
			return;
		};

		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_poSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		auto texture = m_mTextures[oName]->m_poRessource;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		GraphicsHandler::GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, hDescriptor);
	}

}