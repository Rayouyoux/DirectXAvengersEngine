#pragma once
#include "pch.h"
#include "Maths.h"

namespace ave {
	struct ObjectConstants
	{
		DirectX::XMFLOAT4X4 World = maths::MatriceIdentity();
	};

	struct PassConstants
	{
		DirectX::XMFLOAT4X4 View = maths::MatriceIdentity();
		DirectX::XMFLOAT4X4 Proj = maths::MatriceIdentity();
	};
}
