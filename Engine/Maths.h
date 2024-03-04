#pragma once
#include <DirectXPackedVector.h>

using namespace DirectX;

namespace ave 
{

	namespace Maths {

		XMFLOAT4X4 MatriceIdentity();
		const float PI = 3.1415926535f;

		template<typename T>
		T Lerp(T a, T b, float alpha) {
			return a + ((b - a) * alpha);
		}

		static class Random {
		public:
			static float Range(float minInclusive, float maxInclusive) {
				float offset = rand() % (int)((maxInclusive - minInclusive + 1) * 100);
				return minInclusive + offset * 0.01f;
			}

			static int Range(int minInclusive, int maxInclusive) {
				return minInclusive + (rand() % (int)(maxInclusive - minInclusive + 1));
			}
		};
	};
}
