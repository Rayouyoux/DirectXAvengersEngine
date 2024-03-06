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

	}

};