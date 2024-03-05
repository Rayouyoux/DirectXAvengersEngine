#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include <array>

namespace ave {
	Mesh::Mesh() {
		m_poIndexBufferGPU = nullptr;
		m_poVertexBufferGPU = nullptr;
		m_poVertexBufferCPU = nullptr;
		m_poIndexBufferCPU = nullptr;
		m_oIndexFormat = DXGI_FORMAT_R16_UINT;
		m_oIndexBufferByteSize = 0;
		m_oVertexByteStride = 0;
		m_oVertexBufferByteSize = 0;
		m_oIndexCount = 0;
	}
	UINT Mesh::GetIndexCount() {
		return m_oIndexCount;
	}

	D3D12_VERTEX_BUFFER_VIEW Mesh::VertexBufferView()const {
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = m_poVertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = m_oVertexByteStride;
		vbv.SizeInBytes = m_oVertexBufferByteSize;

		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW Mesh::IndexBufferView()const {
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = m_poIndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = m_oIndexFormat;
		ibv.SizeInBytes = m_oIndexBufferByteSize;

		return ibv;
	}

	bool Mesh::BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList){

		std::array<VERTEX_UV, 8> vertices =
		{
			VERTEX_UV({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT2(-1.0f,2.0f)}),
			VERTEX_UV({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT2(-1.0f,2.0f)})
		};

		std::array<std::uint16_t, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(VERTEX_COLOR);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		if (FAILED(D3DCreateBlob(vbByteSize, &m_poVertexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poVertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		if (FAILED(D3DCreateBlob(ibByteSize, &m_poIndexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poIndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		m_poVertexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, vertices.data(), vbByteSize, m_poVertexBufferUploader);

		m_poIndexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, indices.data(), ibByteSize, m_poIndexBufferUploader);

		m_oVertexByteStride = sizeof(VERTEX_COLOR);
		m_oVertexBufferByteSize = vbByteSize;
		m_oIndexFormat = DXGI_FORMAT_R16_UINT;
		m_oIndexBufferByteSize = ibByteSize;

		m_oIndexCount = (UINT)indices.size();

		return true;
	}

	void Mesh::Destroy() {
		m_poVertexBufferCPU->Release();
		m_poIndexBufferCPU->Release();
		m_poIndexBufferGPU->Release();
		m_poVertexBufferGPU->Release();
	}
	Mesh::~Mesh() {
		Destroy();
	}
}
