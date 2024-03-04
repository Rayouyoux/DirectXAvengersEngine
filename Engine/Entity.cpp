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

	Entity::~Entity()
	{}

	void Entity::Initialize(Entity * poParent) {
		m_poTransform = new Transform();
		m_poParent = poParent;
	}

	void Entity::Start() 
	{}

	void Entity::Update(float iDeltaTime) {
		for (int i = 0; i < m_loComponent.size(); i++) {
			m_loComponent[i]->Update(iDeltaTime);
		}
		if(m_poTransform->m_bHandleChange){
			m_poTransform->UpdateMatrice();
			m_poTransform->m_bHandleChange = false;
		}
	}

	void Entity::Render() {
		for (int i = 0; i < m_loComponent.size(); i++) {
			m_loComponent[i]->Render();
		}
	}

	bool Entity::GetIsAlive() {
		return m_bIsAlive;
	}

	void Entity::ToggleIsAlive() {
		m_bIsAlive = !m_bIsAlive;
	}
};
