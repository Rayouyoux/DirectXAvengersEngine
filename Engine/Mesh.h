#pragma once
#include <d3d12.h>
#include <iostream>
#include <string>
#include <vector>
#include "Shape.h"
#include <unordered_map>
#include <functional>

namespace ave {
	class Mesh
	{
	public:
		Mesh();

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const;

		UINT GetIndexCount();
		
		~Mesh();

		template <typename T>
		bool BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList, std::string nameShape) {


			Shape<T>* oShape = new Shape<T>();

			XMVECTOR color = XMVectorSet(0.2f, 0.2f, 0.8f, 1.0f);
			bool isSkybox = true;

			std::unordered_map<std::string, std::function<std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>>()>> shapeCreators = {
				{"cube", [oShape,color, isSkybox]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCube(nullptr,isSkybox);
				}},
				{"cylindre", [oShape]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCylinder(2.0f,5.0f,1.0f,1.0f);
				}},
				{"sphere", [oShape]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateSphere(1.0f,50.0f);
				}},
				{"cone", [oShape]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCone(1.0f,16.0f);
				}},
				{"pyramid", [oShape]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreatePyramid(2.0f,3.0f);
				}},
			};

			std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> aShapes;
			auto it = shapeCreators.find(nameShape);
			if (it != shapeCreators.end()) {
				aShapes = it->second();  // Appel de la fonction de création
			}
			else {
				std::cout << "Invalid shape type." << std::endl;
			}

			const UINT vbByteSize = (UINT)aShapes[0].first.size() * sizeof(T);
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

			m_oVertexByteStride = sizeof(T);
			m_oVertexBufferByteSize = vbByteSize;
			m_oIndexFormat = DXGI_FORMAT_R16_UINT;
			m_oIndexBufferByteSize = ibByteSize;

			m_oIndexCount = (UINT)aShapes[0].second.size();

			return true;
		};

	protected:
		
		ID3DBlob* m_poVertexBufferCPU;
		ID3DBlob* m_poIndexBufferCPU;

		ID3D12Resource* m_poVertexBufferGPU;
		ID3D12Resource* m_poIndexBufferGPU;

		ID3D12Resource* m_poVertexBufferUploader;
		ID3D12Resource* m_poIndexBufferUploader;

		UINT m_oVertexByteStride;
		UINT m_oVertexBufferByteSize;

		DXGI_FORMAT m_oIndexFormat;
		UINT m_oIndexBufferByteSize;

		UINT m_oIndexCount;
	};


}
