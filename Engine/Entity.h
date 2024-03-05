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
		void Render();

		// Adds a Component to the Entity
		template <typename Comp>
		Comp* AddComponent() {
			Comp* poComponent = new Comp();
			m_loComponent.push_back(poComponent);
			poComponent->BindEntity(this);
			/*poComponent->Start();*/
			return poComponent;
		}

		// Gets a Component from the Entity
		template <typename Comp>
		Comp* GetComponent() {
			for (int i = 0; i < m_loComponent.size(); i++) {
				if (typeid(m_loComponent[i]).name() == Comp) {
					return static_cast<Comp>(m_loComponent[i]);
				}
			}
		}

		// Verifies if an Entity has a given Component
		template <typename Comp>
		bool HasComponent() {
			for (int i = 0; i < m_loComponent.size(); i++) {
				if (typeid(m_loComponent[i]) == typeid(Comp)) {
					return true;
				}
			}
			return false;
		}

		// Removes a Component from the Entity
		template <typename Comp>
		bool RemoveComponent() {
			for (auto it = m_loComponent.begin(); it < m_loComponent.end(); it++) {
				if (typeid(*it).name() == Comp) {
					m_loComponent.erase(it);
					return true;
				}
			}
			return false;
		}

		// Is Alive Methods
		bool GetIsAlive();
		void ToggleIsAlive();

	public:

		// Main Public Attributes
		Transform* m_poTransform;
		std::vector<Component*> m_loComponent;
		Entity* m_poParent;
		bool m_bIsAlive;
	};
}

