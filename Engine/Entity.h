#pragma once

#include <typeinfo>
#include <vector>

namespace ave {

	class Transform;
	class Component;

	class Entity {

	public:

		// Constructor
		Entity();

		// Deconstructor
		~Entity();

		// Init
		void Initialize(Entity* poParent = nullptr);

		// SUR Methods
		void Start();
		void Update(float iDeltaTime);
		void LateUpdate(float iDeltaTime);
		void Render();
		void Render2D();

		void OnCollisionEnter(Entity* collider);
		void OnCollisionStay(Entity* collider);
		void OnCollisionExit(Entity* collider);

		// Adds a Component to the Entity
		template <typename Comp>
		Comp* AddComponent() {
			Comp* poComponent = new Comp();
			m_loComponents.push_back(poComponent);
			poComponent->BindEntity(this);
			return poComponent;
		}

		// Gets a Component from the Entity
		template <typename Comp>
		Comp* GetComponent() {
			for (int i = 0; i < m_loComponents.size(); i++) {
				if (typeid(*m_loComponents[i]) == typeid(Comp)) {
					return (Comp*)m_loComponents[i];
				}
			}
		}

		// Verifies if an Entity has a given Component
		template <typename Comp>
		bool HasComponent() {
			for (int i = 0; i < m_loComponents.size(); i++) {
				if (typeid(m_loComponents[i]) == typeid(Comp)) {
					return true;
				}
			}
			return false;
		}

		// Removes a Component from the Entity
		template <typename Comp>
		bool RemoveComponent() {
			for (auto it = m_loComponents.begin(); it < m_loComponents.end(); it++) {
				if (typeid(*it).name() == Comp) {
					m_loComponents.erase(it);
					return true;
				}
			}
			return false;
		}

		// Is Alive Methods
		bool GetIsAlive();
		void ToggleIsAlive();

	public:

		Transform* m_poTransform;
		std::vector<Component*> m_loComponents;
		Entity* m_poParent;
		bool m_bIsAlive;
	};
}