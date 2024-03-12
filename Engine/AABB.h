#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct AABB {
    XMFLOAT3 m_vMin;
    XMFLOAT3 m_vMax;
};