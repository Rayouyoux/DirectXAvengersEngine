#pragma once

#include <vector>

class Transform;
class Component;

namespace ave {
	class Entity {

	public:
		Entity();

		void Start(Transform* poTransform, Entity* poParent);
		void Update();
		void Render();

		~Entity();

		Transform* m_poTransform;
		std::vector<Component> m_loComponent;
		Entity* m_poParent;

		bool m_bHandleChange;
	};
}

