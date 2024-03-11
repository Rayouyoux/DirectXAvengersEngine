#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct AABB {
    XMFLOAT4 pCenter;
    XMFLOAT4 vRadius;
};