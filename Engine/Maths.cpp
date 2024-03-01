#include "pch.h"
#include "Maths.h"

namespace ave 
{

	namespace maths
	{

		XMFLOAT4X4 maths::MatriceIdentity(){

			return { 1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f };
		}

	}

};