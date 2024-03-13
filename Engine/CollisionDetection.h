#pragma once
#include "AABB.h"

namespace ave {

    namespace CollisionDetection {

        bool CheckCollision(const AABB& aabb1, const AABB& aabb2);

    }
}
