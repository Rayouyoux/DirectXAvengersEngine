#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"

namespace ave {

	Entity::Entity() {
		m_poTransform = nullptr;
		m_poParent = nullptr;
		m_bIsAlive = true;
	}

	Entity::~Entity() {
		// Clear all components
		for (int i = 0; i < m_loComponents.size(); i++) {
			delete m_loComponents[i];
		}

		delete m_poTransform;
	}

	void Entity::Initialize(Entity* poParent) {
		m_poTransform = new Transform();
		m_poParent = poParent;
	}

	void Entity::Start() {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Start();
		}
	}

	void Entity::Update(float iDeltaTime) {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Update(iDeltaTime);
		}
		if(m_poTransform->m_bHandleChange){
			m_poTransform->UpdateMatrice();
			m_poTransform->m_bHandleChange = false;
		}
	}

	void Entity::LateUpdate(float dT) {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->LateUpdate(dT);
		}
	}

	void Entity::Render() {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Render();
		}
	}

	void Entity::Render2D() {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Render2D();
		}
	}

	void Entity::OnCollisionEnter(Entity* collider) {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->OnCollisionEnter(collider);
		}
	}

	void Entity::OnCollisionStay(Entity* collider) {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->OnCollisionStay(collider);
		}
	}

	void Entity::OnCollisionExit(Entity* collider) {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->OnCollisionExit(collider);
		}
	}

	void Entity::OnDestroy() {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->OnDestroy();
		}
	}

	bool Entity::GetIsAlive() {
		return m_bIsAlive;
	}

	void Entity::Kill() {
		m_bIsAlive = false;
	}
};
