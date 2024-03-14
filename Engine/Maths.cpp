#include "pch.h"
#include "Maths.h"

namespace ave 
{

	namespace Maths{

		XMFLOAT4X4 Maths::MatriceIdentity(){

			return { 1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f };
		}

		float Maths::TakeLowest(float a, float b) {
			return a > b ? b : a;
		}

		float Maths::TakeHighest(float a, float b) {
			return a > b ? a : b;
		}

	}

};