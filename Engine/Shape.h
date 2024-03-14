#pragma once
#include "pch.h"
using namespace DirectX;
namespace ave {
	template<typename T>
	class Shape
	{
	protected:

		std::vector<T> vertices;
		std::vector < std::uint16_t> indices;

		std::vector<std::pair<std::vector<T>, std::vector<uint16_t>>> m_aShapes;

	public:
		Shape() {};

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateSphere(float fRadius, float fNumSubDivisions, FXMVECTOR* vColor = nullptr) {
			XMFLOAT4 color;

			if (vColor != nullptr) {
				XMStoreFloat4(&color, *vColor);
			}

			for (int i = 0; i <= fNumSubDivisions; i++) {
				float phi = DirectX::XM_PI * i / fNumSubDivisions;
				for (int j = 0; j <= fNumSubDivisions; j++) {
					float theta = 2.0f * DirectX::XM_PI * j / fNumSubDivisions;

					float x = fRadius * sinf(phi) * cosf(theta);
					float y = fRadius * cosf(phi);
					float z = fRadius * sinf(phi) * sinf(theta);

					float u = static_cast<float>(j) / fNumSubDivisions;
					float v = static_cast<float>(i) / fNumSubDivisions;

					T vertex = { DirectX::XMFLOAT3(x,y,z), vColor ? color : DirectX::XMFLOAT4(u,v,0.0f,0.0f) };
					vertices.push_back(vertex);
				}
			}
			for (int i = 0; i < fNumSubDivisions; i++) {
				for (int j = 0; j < fNumSubDivisions; j++) {
					int vertexIndex = i * (fNumSubDivisions + 1) + j;
					indices.push_back(vertexIndex);
					indices.push_back(vertexIndex + 1);
					indices.push_back(vertexIndex + fNumSubDivisions + 1);

					indices.push_back(vertexIndex + 1);
					indices.push_back(vertexIndex + fNumSubDivisions + 2);
					indices.push_back(vertexIndex + fNumSubDivisions + 1);
				}
			}
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateCube(FXMVECTOR* vColor = nullptr, bool isSkybox = false) {
			XMFLOAT4 color;

			if (vColor != nullptr) {
				XMStoreFloat4(&color, *vColor);
			}
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));//Face du haut
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));//Face de bas
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));//Face de droite
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) })); //Face de gauche
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), vColor ? color : XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), vColor ? color : XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}));


			for (int i = 0; i < 6; i++) {
				int index = i * 4;

				indices.push_back(index);
				if(isSkybox == false)
					indices.push_back(index + 1);
				indices.push_back(index + 2);
				if (isSkybox == true)
					indices.push_back(index + 1);

				indices.push_back(index);
				if (isSkybox == false)
					indices.push_back(index + 2);
				indices.push_back(index + 3);
				if (isSkybox == true)
					indices.push_back(index + 2);
			};
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}
		
		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>>CreateCylinder(float fHeight, float fStackCount,float fBottomRadius,float fTopRadius, FXMVECTOR* vColor = nullptr) {

			XMFLOAT4 color;

			if (vColor != nullptr) {
				XMStoreFloat4(&color, *vColor);
			}

			float stackHeight = fHeight / fStackCount;

			float sliceCount = 60.0f;
			// Amount to increment radius as we move up each stack level from bottom to top.
			float radiusStep = (fTopRadius - fBottomRadius) / fStackCount;

			UINT32 ringCount = fStackCount + 1;

			// Compute vertices for each stack ring starting at the bottom and moving up.
			for (UINT32 i = 0; i < ringCount; ++i)
			{
				float y = -0.5f * fHeight + i * stackHeight;
				float r = fBottomRadius + i * radiusStep;

				// vertices of ring
				float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
				for (UINT32 j = 0; j <= sliceCount; ++j)
				{
					T vertex;

					float c = cosf(j * dTheta);
					float s = sinf(j * dTheta);

					//vertex.pos = XMFLOAT3(r * c, y, r * s);

					float u = (float)j / sliceCount;
					float v  = 1.0f - (float)i / fStackCount;

					vertices.push_back(T{ XMFLOAT3(r * c, y, r * s),vColor ? color : XMFLOAT4(u,v,0.0f,0.0f) });
				}
			}

			// Add one because we duplicate the first and last vertex per ring
			// since the texture coordinates are different.
			UINT32 ringVertexCount = sliceCount + 1;

			// Compute indices for each stack.
			for (UINT32 i = 0; i < fStackCount; ++i)
			{
				for (UINT32 j = 0; j < sliceCount; ++j)
				{
					indices.push_back(i * ringVertexCount + j);
					indices.push_back((i + 1) * ringVertexCount + j);
					indices.push_back((i + 1) * ringVertexCount + j + 1);

					indices.push_back(i * ringVertexCount + j);
					indices.push_back((i + 1) * ringVertexCount + j + 1);
					indices.push_back(i * ringVertexCount + j + 1);
				}
			}
			
			CreateBottomDisque(fHeight, sliceCount, fBottomRadius, vColor);
			CreateUpDisque(fHeight, sliceCount, fTopRadius, vColor);
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}
	
		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateCone(float fRadius, float fNumSubDivisions, FXMVECTOR* vColor = nullptr) {
			
			XMFLOAT4 color;

			if (vColor != nullptr) {
				XMStoreFloat4(&color, *vColor);
			}

			float fHeight = 2.0f;

			for (int i = 0; i < fNumSubDivisions; i++) {
				float theta = 2.0f * DirectX::XM_PI * i / fNumSubDivisions;
				float x = fRadius * cosf(theta);
				float y = fRadius * sinf(theta);
				float u = static_cast<float>(i) / (fNumSubDivisions - 1); // Ajustement des coordonnées de texture
				float v = 1.0f;

				vertices.push_back(T{ XMFLOAT3(x, y, 0.0f),vColor ? color : XMFLOAT4(u,v,0.0f,0.0f)});
			}

			// Ajouter le sommet du cône
			vertices.push_back(T{XMFLOAT3(0.0f, 0.0f, fHeight), vColor ? color : XMFLOAT4(0.5f,0.5f,0.0f,0.0f)});

			// Ajouter le centre du disque du bas
			vertices.push_back(T{XMFLOAT3(0.0f, 0.0f, 0.0f), vColor ? color : XMFLOAT4(0.5f,0.5f,0.0f,0.0f)});

			for (int i = 0; i < fNumSubDivisions; ++i) {


				// Indices pour les triangles du disque du bas
				indices.push_back(fNumSubDivisions * 2);
				indices.push_back(fNumSubDivisions + (i + 1) % (int)fNumSubDivisions);
				indices.push_back(fNumSubDivisions + 1);  // Centre du disque du 

				// Indices pour les triangles formés par les points de la base et le sommet
				indices.push_back(i);
				indices.push_back((i + 1) % (int)fNumSubDivisions);
				indices.push_back(fNumSubDivisions);



			}
			
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreatePyramid(float fBaseLength, float fHeight, FXMVECTOR* vColor = nullptr) {
			
			XMFLOAT4 color;

			if (vColor != nullptr) {
				XMStoreFloat4(&color, *vColor);
			}

			// Calcul des coordonnées des vertices
			vertices.push_back(T{ DirectX::XMFLOAT3(-fBaseLength / 2, -fBaseLength / 2, 0.0f),vColor ? color : XMFLOAT4(0.0f,1.0f,0.0f,0.0f)});
			vertices.push_back(T{ DirectX::XMFLOAT3(fBaseLength / 2, -fBaseLength / 2, 0.0f),vColor ? color : XMFLOAT4(1.0f,1.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(fBaseLength / 2, fBaseLength / 2, 0.0f),vColor ? color : XMFLOAT4(0.0f,1.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(-fBaseLength / 2, fBaseLength / 2, 0.0f),vColor ? color : XMFLOAT4(0.0f,0.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(0.0f, 0.0f, fHeight),vColor ? color : XMFLOAT4(0.5f,0.5f,0.0f,0.0f) });

			// Indices pour former les faces de la pyramide
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(4);

			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(4);

			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(4);

			indices.push_back(3);
			indices.push_back(0);
			indices.push_back(4);

			// Indices pour la base de la pyramide
			indices.push_back(0);
			indices.push_back(3);
			indices.push_back(2);

			indices.push_back(0);
			indices.push_back(2);
			indices.push_back(1);

		

			m_aShapes.push_back(std::make_pair(vertices,indices));
			return m_aShapes;
		}
		~Shape();

	protected:
			void CreateBottomDisque(float fHeight, float fSliceCount, float fBottomRadius, FXMVECTOR* vColor = nullptr) {

				XMFLOAT4 color;

				if (vColor != nullptr) {
					XMStoreFloat4(&color, *vColor);
				}
				//Disque du haut
				UINT32 baseIndex = (UINT32)vertices.size();

				float yBottomDisque = -0.5f * fHeight;
				float disqueTheta = 2.0f * XM_PI / fSliceCount;

				// Duplicate cap ring vertices because the texture coordinates and normals differ.
				for (UINT32 i = 0; i <= fSliceCount; ++i)
				{
					float x = fBottomRadius * cosf(i * disqueTheta);
					float z = fBottomRadius * sinf(i * disqueTheta);

					// Scale down by the height to try and make top cap texture coord area
					// proportional to base.
					float u = x / fHeight + 0.5f;
					float v = z / fHeight + 0.5f;

					vertices.push_back(T{ XMFLOAT3(x,yBottomDisque,z),vColor ? color : XMFLOAT4(u,v,0.0f,0.0f) });
				}

				// Cap center vertex.
				vertices.push_back(T{ XMFLOAT3(0.0f,yBottomDisque,0.0f),vColor ? color : XMFLOAT4(0.5f,0.5f,0.0f,0.0f) });

				// Index of center vertex.
				UINT32 centerIndex = (UINT32)vertices.size() - 1;

				for (UINT32 i = 0; i < fSliceCount; ++i)
				{
					indices.push_back(centerIndex);
					indices.push_back(baseIndex + i);
					indices.push_back(baseIndex + i + 1);
				}
			}
			void CreateUpDisque(float fHeight, float fSliceCount, float fTopRadius, FXMVECTOR* vColor = nullptr) {

				XMFLOAT4 color;

				if (vColor != nullptr) {
					XMStoreFloat4(&color, *vColor);
				}

				//Disque du haut
				UINT32 baseIndex = (UINT32)vertices.size();

				float yDisque = 0.5f * fHeight;
				float disqueTheta = 2.0f * XM_PI / fSliceCount;

				// Duplicate cap ring vertices because the texture coordinates and normals differ.
				for (UINT32 i = 0; i <= fSliceCount; ++i)
				{
					float x = fTopRadius * cosf(i * disqueTheta);
					float z = fTopRadius * sinf(i * disqueTheta);

					// Scale down by the height to try and make top cap texture coord area
					// proportional to base.
					float u = x / fHeight + 0.5f;
					float v = z / fHeight + 0.5f;

					vertices.push_back(T{ XMFLOAT3(x,yDisque,z), vColor ? color : XMFLOAT4(u,v,0.0f,0.0f) });
				}

				// Cap center vertex.
				vertices.push_back(T{ XMFLOAT3(0.0f,yDisque,0.0f), vColor ? color : XMFLOAT4(0.5f,0.5f,0.0f,0.0f) });

				// Index of center vertex.
				UINT32 centerIndex = (UINT32)vertices.size() - 1;

				for (UINT32 i = 0; i < fSliceCount; ++i)
				{
					indices.push_back(centerIndex);
					indices.push_back(baseIndex + i + 1);
					indices.push_back(baseIndex + i);
				}
			}
	};


}
