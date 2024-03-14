#pragma once
#include "pch.h"
#include "Maths.h"

namespace ave {
	struct ObjectConstants
	{
		DirectX::XMFLOAT4X4 World = Maths::MatriceIdentity();
		DirectX::XMFLOAT4 Color;
	};

	struct PassConstants
	{
		DirectX::XMFLOAT4X4 View = Maths::MatriceIdentity();
		DirectX::XMFLOAT4X4 Proj = Maths::MatriceIdentity();
	};
}
