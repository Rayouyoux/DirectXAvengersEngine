#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include <array>
#include <vector>
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

		//std::array<VERTEX_UV, 24> vertices =
		//{
		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, +1.0f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),


		//};

		//std::array<std::uint16_t, 36> indices =
		//{
		//	// front face
		//	0, 1, 2,
		//	0, 2, 3,

		//	// back face
		//	4, 5, 6,
		//	4, 6, 7,

		//	8,9,10,
		//	8,10,11,

		//	12,13,14,
		//	12,14,15,

		//	16,17,18,
		//	16,18,19,

		//	20,21,22,
		//	20,22,23
		//};

		//la height
		float height = 10.0f;
		//le nombre de stack
		float stackCount = 4.0f;
		float topRadius = 10.0f;
		float bottomRadius = 10.0f;
		//la hauteur de chaque stack
		float stackHeight = height / stackCount;
		
		float sliceCount = 8.0f;
		//pour incrémenter le radius de bas en haut si il est différent
		float radiusStep = (topRadius / bottomRadius) / stackCount;

		//le nombre d'anneau
		float ringCount = stackCount + 1;
		

		std::vector<VERTEX_COLOR>vertices;

		for (UINT32 i = 0; i < ringCount; i++) {
			
			float y = -0.5 * height + i * stackHeight;

			float r = bottomRadius + i * radiusStep;

			//Vertices of ring
			float dTheta = 2.0f * XM_PI / sliceCount;

			for (UINT32 j = 0; j < sliceCount; j++) {
				VERTEX_COLOR test;
				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				test.pos = XMFLOAT3(r * c, y, r * s);

				test.color.x = (float)j / sliceCount;
				test.color.y = 1.0f - (float)i / stackCount;

				vertices.push_back(test);
			}
		}
		std::vector<std::uint16_t> indices;
		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		UINT32 ringVertexCount = sliceCount + 1;

		for (UINT32 i = 0; i < stackCount; i++) {
			for (UINT32 j = 0; j < sliceCount; j++) {
				indices.push_back(i * ringVertexCount + j);
				indices.push_back((i + 1) * ringVertexCount + j);
				indices.push_back((i + 1 ) * ringVertexCount + j + 1);

				indices.push_back(i * ringVertexCount + j);
				indices.push_back((i + 1) * ringVertexCount + j + 1);
				indices.push_back(i * ringVertexCount + j + 1);
			}
		}

		UINT32 baseIndex = (UINT32)vertices.size();

		float a = 0.5f * height;
		float dTheta = 2.0f * XM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (UINT32 i = 0; i <= sliceCount; ++i)
		{
			VERTEX_COLOR vertex;

			vertex.pos.x = topRadius * cosf(i * dTheta);
			vertex.pos.y = a;
			vertex.pos.z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			vertex.color.x = vertex.pos.x / height + 0.5f;
			vertex.color.y = vertex.pos.z / height + 0.5f;

			vertices.push_back(vertex);
		}

		// Cap center vertex.
		vertices.push_back(VERTEX_COLOR{ XMFLOAT3(0.0f, a, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) });

		// Index of center vertex.
		UINT32 centerIndex = (UINT32)vertices.size() - 1;

		for (UINT32 i = 0; i < sliceCount; ++i)
		{
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}

		UINT32 IndexBase = (UINT32)vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta1 = 2.0f * XM_PI / sliceCount;
		for (UINT32 i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta1);
			float z = bottomRadius * sinf(i * dTheta1);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			vertices.push_back(VERTEX_COLOR{ XMFLOAT3(x, y, z), XMFLOAT4(u,v,0,0) });
		}

		// Cap center vertex.
		vertices.push_back(VERTEX_COLOR{ XMFLOAT3(0.0f, y, 0.0f),XMFLOAT4( 0.0f, -1.0f, 0.0f, 1.0f)});

		// Cache the index of center vertex.
		UINT32 IndexCentre = (UINT32)vertices.size() - 1;

		for (UINT32 i = 0; i < sliceCount; ++i)
		{
			indices.push_back(IndexCentre);
			indices.push_back(IndexBase + i);
			indices.push_back(IndexBase + i + 1);
		}

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(VERTEX_UV);
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

		m_oVertexByteStride = sizeof(VERTEX_UV);
		m_oVertexBufferByteSize = vbByteSize;
		m_oIndexFormat = DXGI_FORMAT_R16_UINT;
		m_oIndexBufferByteSize = ibByteSize;

		m_oIndexCount = (UINT)indices.size();

		return true;
	};

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
