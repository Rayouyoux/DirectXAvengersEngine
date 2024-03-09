#include "pch.h"
#include "Mesh.h"
#include "Vertex.h"
#include <array>
#include <vector>
#include "Shape.h"
#include <unordered_map>
#include <functional>

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


	bool Mesh::BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList, std::string nameShape) {


		Shape<VERTEX_UV>* oShape = new Shape<VERTEX_UV>();
		
		std::unordered_map<std::string, std::function<std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>>()>> shapeCreators = {
			{"cube", [oShape]() -> std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> {
				return oShape->CreateCube();
			}},
			{"cylindre", [oShape]() -> std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> {
				return oShape->CreateCylinder(70);
			}},
			{"sphere", [oShape]() -> std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> {
				return oShape->CreateSphere(1.0f,60.0f);
			}},
			{"cone", [oShape]() -> std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> {
				return oShape->CreateCone(1.0f,16.0f);
			}},
			{"pyramid", [oShape]() -> std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> {
				return oShape->CreatePyramid(2.0f,3.0f);
			}},
		};

		std::vector<std::pair<std::vector<VERTEX_UV>, std::vector<uint16_t>>> aShapes;
		auto it = shapeCreators.find(nameShape);
		if (it != shapeCreators.end()) {
			aShapes = it->second();  // Appel de la fonction de création
		}
		else {
			std::cout << "Invalid shape type." << std::endl;
		}
		
		const UINT vbByteSize = (UINT)aShapes[0].first.size() * sizeof(VERTEX_UV);
		const UINT ibByteSize = (UINT)aShapes[0].second.size() * sizeof(std::uint16_t);

		if (FAILED(D3DCreateBlob(vbByteSize, &m_poVertexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poVertexBufferCPU->GetBufferPointer(), aShapes[0].first.data(), vbByteSize);

		if (FAILED(D3DCreateBlob(ibByteSize, &m_poIndexBufferCPU))) {
			return false;
		}
		CopyMemory(m_poIndexBufferCPU->GetBufferPointer(), aShapes[0].second.data(), ibByteSize);

		m_poVertexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, aShapes[0].first.data(), vbByteSize, m_poVertexBufferUploader);

		m_poIndexBufferGPU = D3DUtils::CreateDefaultBuffer(poDevice,
			poCommandList, aShapes[0].second.data(), ibByteSize, m_poIndexBufferUploader);

		m_oVertexByteStride = sizeof(VERTEX_UV);
		m_oVertexBufferByteSize = vbByteSize;
		m_oIndexFormat = DXGI_FORMAT_R16_UINT;
		m_oIndexBufferByteSize = ibByteSize;

		m_oIndexCount = (UINT)aShapes[0].second.size();

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
