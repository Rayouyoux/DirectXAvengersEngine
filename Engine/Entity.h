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

		void Start(Transform* poTransform, Entity* poParent = nullptr);
		void Update(float iDeltaTime);
		void Render();

		template <typename Comp>
		void AddComponent();
		template <typename Comp>
		Component* GetComponent();
		template <typename Comp>
		void RemoveComponent();

		Transform* m_poTransform;
		std::vector<Component*> m_loComponent;
		Entity* m_poParent;
	};
}

