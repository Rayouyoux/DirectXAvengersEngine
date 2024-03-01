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
		void Update(float iDeltaTime);
		void Render();

		template <typename Comp>
		Comp* AddComponent() {
			Comp* poComponent = new Comp();
			m_loComponent.push_back(poComponent);
			poComponent->BindEntity(this);
			poComponent->Start();
			return poComponent;
		}

		template <typename T>
		T* GetComponent() {
			for (int i = 0; i < m_loComponent.size(); i++) {
				if (typeid(m_loComponent[i]).name() == T) {
					return static_cast<T>(m_loComponent[i]);
				}
			}
		}

		template <typename T>
		bool RemoveComponent() {
			for (auto it = m_loComponent.begin(); it < m_loComponent.end(); it++) {
				if (typeid(*it).name() == T) {
					m_loComponent.erase(it);
					return true;
				}
			}
			return false;
		}

		Transform* m_poTransform;
		std::vector<Component*> m_loComponent;
		Entity* m_poParent;
	};
}

