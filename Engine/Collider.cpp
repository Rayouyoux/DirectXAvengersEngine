#include "pch.h"
#include "Collider.h"
#include "CollisionDetection.h"
#include "Entity.h"
#include "AABB.h"
#include "Mesh.h"
#include "MeshRenderer.h"

namespace ave {

    Collider::Collider() {
        m_vCollidables.push_back(this);
    }

    Collider::~Collider() {
        auto it = std::find(m_vCollidables.begin(), m_vCollidables.end(), this);
        m_vCollidables.erase(it);
    }

    void Collider::Start()
    {}

    void Collider::Update(float deltaTime)
    {}

    void Collider::LateUpdate(float deltaTime)
    {
        for (auto collider : m_vCollidables) {

            if (collider == this) continue;

            AABB& AABB1 = *m_poEntity->GetComponent<MeshRenderer>()->GetMesh()->GetAABB();
            AABB& AABB2 = *collider->m_poEntity->GetComponent<MeshRenderer>()->GetMesh()->GetAABB();
            bool isColliding = CollisionDetection::CheckCollision(AABB1, AABB2);
            
            auto it = m_mColliding.find(collider);
            CollideState prevState = (it == m_mColliding.end()) ? CollideState::Away : m_mColliding[collider];
            
            CollideState currentState = CollideState::Away;
            if (prevState == CollideState::Away) {
                currentState = isColliding ? CollideState::Enter : CollideState::Away;
            }
            else if (prevState == CollideState::Enter) {
                currentState = isColliding ? CollideState::Stay : CollideState::Exit;
            }
            else if (prevState == CollideState::Stay) {
                currentState = isColliding ? CollideState::Stay : CollideState::Exit;
            }
            else if (prevState == CollideState::Exit) {
                currentState = isColliding ? CollideState::Enter : CollideState::Away;
            }

            switch (currentState) {
            case CollideState::Enter:
                m_poEntity->OnCollisionEnter(collider->m_poEntity);
                break;
            case CollideState::Stay:
                m_poEntity->OnCollisionStay(collider->m_poEntity);
                break;
            case CollideState::Exit:
                m_poEntity->OnCollisionExit(collider->m_poEntity);
                break;
            }
        }
    }

    void Collider::Render()
    {}

    void Collider::OnCollisionEnter(Entity* collider) {
        for (int i = 0; i < m_mColliding.size(); i++)
        {

        }
    }

    void Collider::OnCollisionStay(Entity* collider) {
        for (int i = 0; i < m_mColliding.size(); i++)
        {

        }
    }

    void Collider::OnCollisionExit(Entity* collider) {
        for (int i = 0; i < m_mColliding.size(); i++)
        {
            
        }
    }

}
