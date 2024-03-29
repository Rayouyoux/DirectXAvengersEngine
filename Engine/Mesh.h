#pragma once
#include <d3d12.h>
#include <iostream>
#include <string>
#include <vector>
#include "Shape.h"
#include <unordered_map>
#include <functional>
#include "AABB.h"

namespace ave {

	class Mesh
	{
	public:
		Mesh();

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const;

		UINT GetIndexCount();

		AABB* GetAABB();

		void SetAABB(XMFLOAT3 vMin, XMFLOAT3 vMax);

		void SetAABB(AABB oAABB);
		void SetVertice(XMFLOAT3 vVertice);
		std::vector<XMFLOAT3> GetVertices();

		std::vector<XMFLOAT3> m_vVertices;
		
		~Mesh();

		template <typename T>
		bool BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList, std::string nameShape, FXMVECTOR* oColor = nullptr) {

			Shape<T>* oShape = new Shape<T>();

			float fCubeSidesSize = 0.5f;
			float fSphereRadius = 1.0f;
			float fSphereNumSubDivisions = 50.0f;
			float fCylinderHeight = 2.0f;
			float fCylinderStackCount = 5.0f;
			float fCylinderBottomRadius = 1.0f;
			float fCylinderTopRadius = fCylinderBottomRadius;
			float fConeRadius = 1.0f;
			float fConeNumSubDivisions = 16.0f;
			float fConeHeight = 2.0f;
			float fPyramidBaseLength = 2.0f;
			float fPyramidHeight = 3.0f;

			if (nameShape == "cube") {
				m_oContainingBox.m_vMin = XMFLOAT3(-fCubeSidesSize, -fCubeSidesSize, -fCubeSidesSize);
				m_oContainingBox.m_vMax = XMFLOAT3(fCubeSidesSize, fCubeSidesSize, fCubeSidesSize);
			}
			else if (nameShape == "cylindre") {
				m_oContainingBox.m_vMin = XMFLOAT3(-fCylinderBottomRadius, -fCylinderHeight / 2, -fCylinderBottomRadius);
				m_oContainingBox.m_vMax = XMFLOAT3(fCylinderBottomRadius, fCylinderHeight / 2, fCylinderBottomRadius);
			}
			else if (nameShape == "sphere") {
				m_oContainingBox.m_vMin = XMFLOAT3(-fSphereRadius, -fSphereRadius, -fSphereRadius);
				m_oContainingBox.m_vMax = XMFLOAT3(fSphereRadius, fSphereRadius, fSphereRadius);
			}
			else if (nameShape == "cone") {
				m_oContainingBox.m_vMin = XMFLOAT3(-fConeRadius, 0.0f, -fConeRadius);
				m_oContainingBox.m_vMax = XMFLOAT3(fConeRadius, fConeHeight, fConeRadius);
			}
			else if (nameShape == "pyramid") {
				m_oContainingBox.m_vMin = XMFLOAT3(-fPyramidBaseLength / 2, 0.0f, -fPyramidBaseLength / 2);
				m_oContainingBox.m_vMax = XMFLOAT3(fPyramidBaseLength, fPyramidHeight, fPyramidBaseLength);
			}
			
			std::unordered_map<std::string, std::function<std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>>()>> shapeCreators = {
				{"cube", [oShape, oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCube(oColor,false);
				}},
				{"cylindre", [oShape,oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCylinder(2.0f,5.0f,1.0f,1.0f,oColor);
				}},
				{"sphere", [oShape,oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateSphere(1.0f,50.0f,oColor);
				}},
				{"cone", [oShape,oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCone(1.0f,16.0f,oColor);
				}},
				{"pyramid", [oShape,oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreatePyramid(2.0f,3.0f,oColor);
				}},
				{"skybox", [oShape,oColor]() -> std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> {
					return oShape->CreateCube(oColor,true);
				}},

			};

			std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> aShapes;
			
			auto it = shapeCreators.find(nameShape);
			if (it != shapeCreators.end()) {
				aShapes = it->second();  // Appel de la fonction de cr�ation
			}
			else {
				std::cout << "Invalid shape type." << std::endl;
			}
	
			for (int i = 0; i < aShapes.size(); i++) {
				this->SetVertice(aShapes[i].first[0].pos);
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

		AABB m_oContainingBox;
	};
}
