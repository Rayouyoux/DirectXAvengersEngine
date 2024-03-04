#pragma once
#include <DirectXPackedVector.h>
#include <time.h>

using namespace DirectX;

namespace ave {

	namespace Maths {

		XMFLOAT4X4 MatriceIdentity();

		template<typename T>
		T Lerp(T a, T b, float alpha) {
			return a + ((b - a) * alpha);
		}

		static class Random {
		private:
			static bool m_bInitialized;

			static void Initialize() {
				m_bInitialized = true;
				srand(time(0));
			}

		public:
			template<typename T>
			static T Range(T minInclusive, T maxInclusive) {
				if (m_bInitialized == false)
					Initialize();
				return minInclusive + (rand() % (maxInclusive - minInclusive + 1));
			}
		};
	};
}
