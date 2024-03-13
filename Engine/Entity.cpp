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
		if(m_poTransform->m_bHandleChange){
			m_poTransform->UpdateMatrice();
			m_poTransform->m_bHandleChange = false;
		}
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Update(iDeltaTime);
		}
	}

	void Entity::Render() {
		for (int i = 0; i < m_loComponents.size(); i++) {
			m_loComponents[i]->Render();
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

	bool Entity::GetIsAlive() {
		return m_bIsAlive;
	}

	void Entity::ToggleIsAlive() {
		m_bIsAlive = !m_bIsAlive;
	}
};
