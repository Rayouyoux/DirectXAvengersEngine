#pragma once
#include "pch.h"
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

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateSphere(float fRadius, float fNumSubDivisions) {
			for (int i = 0; i <= fNumSubDivisions; i++) {
				float phi = DirectX::XM_PI * i / fNumSubDivisions;
				for (int j = 0; j <= fNumSubDivisions; j++) {
					float theta = 2.0f * DirectX::XM_PI * j / fNumSubDivisions;

					float x = fRadius * sinf(phi) * cosf(theta);
					float y = fRadius * cosf(phi);
					float z = fRadius * sinf(phi) * sinf(theta);

					T vertex = { DirectX::XMFLOAT3(x,y,z), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,0.0f) };
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

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateCube() {

			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f) }));

			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT4(1.0f, 0.0f,0.0f,0.0f) }));
			vertices.push_back(T({ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT4(1.0f, 1.0f,0.0f,0.0f)}));


			for (int i = 0; i < 6; i++) {
				int index = i * 4;

				indices.push_back(index);
				indices.push_back(index + 1);
				indices.push_back(index + 2);

				indices.push_back(index);
				indices.push_back(index + 2);
				indices.push_back(index + 3);
			};
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}
		
		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>>CreateCylinder(UINT uPartCount) {

			float pipeVerticesCount = uPartCount * 2 + 2;
			std::vector<T> vertices;
			UINT pipeIndicesCount = uPartCount * 4 * 3;
			std::vector<std::uint16_t> indices;
			int index = 0;

			// On créer les indices du disque du haut 0 étant le point centrale donc tout les
			// triangles sont fait à partir du centre
			for (UINT i = 0; i < uPartCount; i++) {
				indices.push_back(0);
				if (i == uPartCount - 1)
					indices.push_back(1);
				else
					indices.push_back((i + 1) % uPartCount + 1);
				indices.push_back(i + 1);
			}

			// On créer les indices du disque du bas 11 étant le point centrale 
			// c'est pour sa qu'on fait le start.
			UINT start = pipeVerticesCount / 2;
			for (UINT i = start, j = 0; j < uPartCount; i++, j++) {
				indices.push_back(start);
				indices.push_back(i + 1);
				if (j == uPartCount - 1)
					indices.push_back(start + 1);
				else
					indices.push_back(i + 2);
			}

			// Side Rectangles
			for (UINT i = 0; i < uPartCount; i++) {
				indices.push_back(i + 1);
				if (i == uPartCount - 1)
					indices.push_back(1);
				else
					indices.push_back(i + 2);
				indices.push_back(i + start + 1);

				if (i == uPartCount - 1)
				{
					indices.push_back(1);
					indices.push_back(1 + start);
				}
				else
				{
					indices.push_back(i + 2);
					indices.push_back(i + start + 2);
				}
				indices.push_back(i + start + 1);

			}

			float x, z, angle;
			// Center Vertices
			vertices.push_back(T{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f,0.0f,0.0f,0.0f)});

			// Disks Vertcies
			float step = XM_2PI / uPartCount;
			for (int disc = 0; disc < 2; disc++)
			{
				for (float angle = 0.0f; angle < XM_2PI; angle += step)
				{
					x = cosf(angle);
					z = sinf(angle);
					vertices.push_back(T{ XMFLOAT3(x, disc == 0 ? 1.0f : -1.0f, z), XMFLOAT4(0.0f,1.0f,0.0f,0.0f)});
				}
				vertices.push_back(T{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1.0f,1.0f,0.0f,0.0f) });
			}
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateCone(float fRadius, float fNumSubDivisions) {
			
			float fHeight = 2.0f;

			for (int i = 0; i < fNumSubDivisions; i++) {
				float theta = 2.0f * DirectX::XM_PI * i / fNumSubDivisions;
				float x = fRadius * cosf(theta);
				float y = fRadius * sinf(theta);
				vertices.push_back(T{ XMFLOAT3(x, y, 0.0f), XMFLOAT4(1.0f,0.0f,0.0f,0.0f)});
			}

			// Ajouter le sommet du cône
			vertices.push_back(T{XMFLOAT3(0.0f, 0.0f, fHeight), XMFLOAT4(0.0f,1.0f,0.0f,0.0f)});

			// Ajouter le centre du disque du bas
			vertices.push_back(T{XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f,1.0f,0.0f,0.0f)});

			for (int i = 0; i < fNumSubDivisions; ++i) {
				// Indices pour les triangles formés par les points de la base et le sommet
				indices.push_back(i);
				indices.push_back((i + 1) % (int)fNumSubDivisions);
				indices.push_back(fNumSubDivisions);

				// Indices pour les triangles du disque du bas
				indices.push_back(fNumSubDivisions * 2);
				indices.push_back(fNumSubDivisions + (i + 1) % (int)fNumSubDivisions);
				indices.push_back(fNumSubDivisions + 1);  // Centre du disque du bas
			}
			
			m_aShapes.push_back(std::make_pair(vertices, indices));
			return m_aShapes;
		}

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreatePyramid(float fBaseLength, float fHeight) {
			// Calcul des coordonnées des vertices
			vertices.push_back(T{ DirectX::XMFLOAT3(-fBaseLength / 2, -fBaseLength / 2, 0.0f), XMFLOAT4(1.0f,0.0f,0.0f,0.0f)});
			vertices.push_back(T{ DirectX::XMFLOAT3(fBaseLength / 2, -fBaseLength / 2, 0.0f), XMFLOAT4(1.0f,1.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(fBaseLength / 2, fBaseLength / 2, 0.0f), XMFLOAT4(0.0f,1.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(-fBaseLength / 2, fBaseLength / 2, 0.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f) });
			vertices.push_back(T{ DirectX::XMFLOAT3(0.0f, 0.0f, fHeight), XMFLOAT4(1.0f,0.0f,0.0f,0.0f) });

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
			indices.push_back(1);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);

			m_aShapes.push_back(std::make_pair(vertices,indices));
			return m_aShapes;
		}
		~Shape();
	};


}
