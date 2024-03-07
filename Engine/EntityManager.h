#pragma once

#include "Entity.h"
#include <vector>

namespace ave {

	class EntityManager
	{
	public:

		// Constructor
		EntityManager();

		// Deconstructor
		~EntityManager();

		// SUR Methods
		void Update(float iDeltaTime);
		void LateUpdate();
		void Render();

		// Returns a vector of Entities that have the given Component
		template <typename Comp>
		std::vector<Entity*> GetEntitiesByComponent() {
			std::vector<Entity*> Entities;
			for (int i = 0; i < m_voAliveEntities.size(); i++) {
				if (m_voAliveEntities[i]->HasComponent<Comp>()) {
					Entities.push_back(m_voAliveEntities[i]);
				}
			}
			return Entities;
		}

		// Main Methods
		Entity* NewEntity();
		bool RegisterEntity(Entity* poEntity);

	public:

		// Main Public Members
		std::vector<Entity*> m_voAliveEntities;

	};

}

