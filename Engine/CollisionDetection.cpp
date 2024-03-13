#include "pch.h"
#include "CollisionDetection.h"

namespace ave {

    namespace CollisionDetection {
        bool CheckCollision(const AABB& aabb1, const AABB& aabb2) {

            if (aabb1.m_vMax.x < aabb2.m_vMin.x || aabb1.m_vMin.x > aabb2.m_vMax.x) return false;
            if (aabb1.m_vMax.y < aabb2.m_vMin.y || aabb1.m_vMin.y > aabb2.m_vMax.y) return false;
            if (aabb1.m_vMax.z < aabb2.m_vMin.z || aabb1.m_vMin.z > aabb2.m_vMax.z) return false;

            return true;
        }
    }

}