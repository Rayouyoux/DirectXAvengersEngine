#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Transform.h"

namespace ave {

	Entity::Entity() {
		m_poTransform = nullptr;
		m_poParent = nullptr;
	}

	Entity::~Entity() {

	}

	void Entity::Start(Transform* poTransform, Entity* poParent) {
		m_poTransform = poTransform;
		m_poParent = poParent;
	}

	void Entity::Update() {
		for (int i = 0; i < m_loComponent.size(); i++) {
			m_loComponent[i]->Update();
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

	template <typename Comp>
	void Entity::AddComponent() {
		Comp* poComponent = new Comp();
		m_loComponent.push_back(poComponent);
		poComponent->BindEntity();
		poComponent->Start();
	}

	template <typename Comp>
	Component* Entity::GetComponent() {
		for (int i = 0; i < m_loComponent.size(); i++) {
			if (typeid(m_loComponent[i]).name() == Comp) {
				return m_loComponent[i];
			}
		}
	}

	template <typename Comp>
	void Entity::RemoveComponent() {
		for (auto it = m_loComponent.begin(); it < m_loComponent.end(); it++) {
			if (typeid(*it).name() == Comp) {
				m_loComponent.erase(it);
			}
		}
	}
};
