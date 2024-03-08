#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include <array>
#include <vector>
#include "Shape.h"
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

	//void Mesh::CreateCylinder(UINT uPartCount) {
	//	UINT partCount = 50;
	//	float pipeVerticesCount = partCount * 2 + 2;
	//	std::vector<VERTEX_UV> vertices;
	//	UINT pipeIndicesCount = partCount * 4 * 3;
	//	std::vector<std::uint16_t> indices;
	//	int index = 0;


	//	// On créer les indices du disque du haut 0 étant le point centrale donc tout les
	//	// triangles sont fait à partir du centre
	//	for (UINT i = 0; i < partCount; i++) {
	//		indices.push_back(0);
	//		if (i == partCount - 1)
	//			indices.push_back(1);
	//		else
	//			indices.push_back((i + 1) % partCount + 1);
	//		indices.push_back(i + 1);
	//	}

	//	// On créer les indices du disque du bas 11 étant le point centrale 
	//	// c'est pour sa qu'on fait le start.
	//	UINT start = pipeVerticesCount / 2;
	//	for (UINT i = start, j = 0; j < partCount; i++, j++) {
	//		indices.push_back(start);
	//		indices.push_back(i + 1);
	//		if (j == partCount - 1)
	//			indices.push_back(start + 1);
	//		else
	//			indices.push_back(i + 2);
	//	}

	//	// Side Rectangles
	//	for (UINT i = 0; i < partCount; i++) {
	//		indices.push_back(i + 1);
	//		if (i == partCount - 1)
	//			indices.push_back(1);
	//		else
	//			indices.push_back(i + 2);
	//		indices.push_back(i + start + 1);

	//		if (i == partCount - 1)
	//		{
	//			indices.push_back(1);
	//			indices.push_back(1 + start);
	//		}
	//		else
	//		{
	//			indices.push_back(i + 2);
	//			indices.push_back(i + start + 2);
	//		}
	//		indices.push_back(i + start + 1);

	//	}

	//	float x, z, angle;
	//	// Center Vertices
	//	vertices.push_back(VERTEX_COLOR{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::DarkRed) });

	//	// Disks Vertcies
	//	float step = XM_2PI / partCount;
	//	for (int disc = 0; disc < 2; disc++)
	//	{
	//		for (float angle = 0.0f; angle < XM_2PI; angle += step)
	//		{
	//			x = cosf(angle);
	//			z = sinf(angle);
	//			vertices.push_back(VERTEX_COLOR{ XMFLOAT3(x, disc == 0 ? 1.0f : -1.0f, z), XMFLOAT4(Colors::White) });
	//		}
	//		vertices.push_back(VERTEX_COLOR{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Blue) });
	//	}

	//}

	bool Mesh::BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList){

		//std::array<VERTEX_UV, 24> vertices =
		//{

		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),

		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f)}),
		//	VERTEX_UV({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f)}),

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

		Shape<VERTEX_COLOR>* oShape = new Shape<VERTEX_COLOR>();

		std::vector<std::pair<std::vector<VERTEX_COLOR>, std::vector<uint16_t>>> test;
		test = oShape->CreateSphere(1.0f, 60.0f);
		
		const UINT vbByteSize = (UINT)test[0].first.size() * sizeof(VERTEX_COLOR);
		const UINT ibByteSize = (UINT)test[0].second.size() * sizeof(std::uint16_t);

		if (FAILED(D3DCreateBlob(vbByteSize, &m_poVertexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poVertexBufferCPU->GetBufferPointer(), test[0].first.data(), vbByteSize);

		if (FAILED(D3DCreateBlob(ibByteSize, &m_poIndexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poIndexBufferCPU->GetBufferPointer(), test[0].second.data(), ibByteSize);

		m_poVertexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, test[0].first.data(), vbByteSize, m_poVertexBufferUploader);

		m_poIndexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, test[0].second.data(), ibByteSize, m_poIndexBufferUploader);

		m_oVertexByteStride = sizeof(VERTEX_UV);
		m_oVertexBufferByteSize = vbByteSize;
		m_oIndexFormat = DXGI_FORMAT_R16_UINT;
		m_oIndexBufferByteSize = ibByteSize;

		m_oIndexCount = (UINT)test[0].second.size();

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
