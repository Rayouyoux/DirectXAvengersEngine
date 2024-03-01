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
		Component* GetComponent();
		template <class Comp>
		void RemoveComponent();

		Transform* m_poTransform;
		std::vector<Component*> m_loComponent;
		Entity* m_poParent;
	};
}

