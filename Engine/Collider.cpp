#include "pch.h"
#include "Collider.h"
#include "Entity.h"

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
            bool isColliding = true;//(bool)CollidesWith(collider);
            CollideState currentState = m_mColliding[collider];
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
