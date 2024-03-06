#pragma once
#include <DirectXPackedVector.h>

using namespace DirectX;

namespace ave 
{

	namespace maths 
	{

		XMFLOAT4X4 MatriceIdentity();
		const float PI = 3.1415926535f;

		template<typename T>
		T Clamp(const T& x, const T& low, const T& high)
		{
			return x < low ? low : (x > high ? high : x);
		}

	};
}
