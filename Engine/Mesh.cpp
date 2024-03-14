#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include "AABB.h"
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

	ABBB* Mesh::GetAABB() {
		return &m_oContainingBox;
	}

	void Mesh::SetAABB(XMFLOAT3 vMin, XMFLOAT3 vMax) {
		m_oContainingBox.m_vMin = vMin;
		m_oContainingBox.m_vMax = vMax;
	}

	void Mesh::SetAABB(AABB oAABB) {
		m_oContainingBox = oAABB;
	}

	void Mesh::SetVertice(XMFLOAT3 vVertice) {
		m_vVertices.push_back(vVertice);
	}

	std::vector<XMFLOAT3> Mesh::GetVertices() {
		return m_vVertices;
	}

	Mesh::~Mesh() {
		m_poVertexBufferCPU->Release();
		m_poIndexBufferCPU->Release();
		m_poVertexBufferGPU->Release();
		m_poIndexBufferGPU->Release();

		m_poVertexBufferUploader->Release();
		m_poIndexBufferUploader->Release();
	}
}
