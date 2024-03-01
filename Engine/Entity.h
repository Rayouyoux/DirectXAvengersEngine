#pragma once

#include <typeinfo>
#include <vector>

namespace ave {

	class Transform;
	class Component;

	class Entity {

	public:
		Entity();
		~Entity();

		void Start(Entity* poParent = nullptr);
		void Update();
		void Render();

		template <class Comp>
		Comp* AddComponent() {
			Comp* poComponent = new Comp();
			m_loComponent.push_back(poComponent);
			poComponent->BindEntity(this);
			poComponent->Start();
			return poComponent;
		}

		template <class Comp>
		Component* GetComponent() {
			for (int i = 0; i < m_loComponent.size(); i++) {
				if (typeid(m_loComponent[i]).name() == Comp) {
					return m_loComponent[i];
				}
			}
		}

		template <class Comp>
		void RemoveComponent() {
			for (auto it = m_loComponent.begin(); it < m_loComponent.end(); it++) {
				if (typeid(*it).name() == Comp) {
					m_loComponent.erase(it);
				}
			}
		}

		Transform* m_poTransform;
		std::vector<Component*> m_loComponent;
		Entity* m_poParent;
	};
}

