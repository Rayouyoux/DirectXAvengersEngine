#pragma once

#include <vector>

class Transform;
class Component;

namespace ave {

	class Entity {

	public:
		Entity();
		~Entity();

		void Start(Transform* poTransform, Entity* poParent);
		void Update();
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

