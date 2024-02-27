#include "pch.h"
#include "Entity.h"
#include "Transform.h"


namespace ave {

	Entity::Entity() {
		m_poTransform = nullptr;
		m_poParent = nullptr;
		m_bHandleChange = false;
	}
	void Entity::Start(Transform* poTransform, Entity* poParent = nullptr) {
		m_poTransform = poTransform;
		m_poParent = poParent;
	}
	void Entity::Update() {
		if(m_bHandleChange){
			m_poTransform->UpdateMatrice();
		}
	}
	void Entity::Render() {
		
	}

	Entity::~Entity() {

	}
};
