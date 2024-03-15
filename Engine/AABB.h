#pragma once

#include <DirectXMath.h>

using namespace DirectX;

namespace ave {
    struct AABB {
        XMFLOAT3 m_vMin;
        XMFLOAT3 m_vMax;
    };
}
