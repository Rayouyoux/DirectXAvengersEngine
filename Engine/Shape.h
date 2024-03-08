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
		Shape();

		std::vector < std::pair<std::vector<T>, std::vector<uint16_t>>> CreateSphere(float fRadius, float fNumSubDivisions) {
			for (int i = 0; i < fNumSubDivisions; i++) {
				float phi = DirectX::XM_PI * i / fNumSubDivisions;
				for (int j = 0; j < fNumSubDivisions; j++) {
					float theta = 2.0f * DirectX::XM_PI * j / fNumSubDivisions;

					float x = fRadius * cosf(phi) * sinf(theta);
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
		std::vector<Shape*> CreateCube();
		std::vector<Shape*> CreateCylinder(UINT uPartCount);
		~Shape();
	};


}
