#pragma once

#include "Entity.h"
#include <vector>

namespace ave {
	class AvengersEngine;
	class Shader;
	class GraphicsHandler;
	class Mesh;
	class Camera;

	class EntityManager
	{
	public:

		// Main Public Members
		std::vector<Entity*> m_voAliveEntities;

	protected:
		Camera* m_poMainCamera;
		AvengersEngine* m_poGameInstance;
		GraphicsHandler* m_poGraphics;

	public:

		// Constructor
		EntityManager();

		// Deconstructor
		~EntityManager();

		/*static EntityManager* Create();
		bool Initialize(HINSTANCE hInstance);*/
		/*int Run();*/

		void Init(GraphicsHandler* poGraphics);

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

		Entity* GetMainCamera();

		// Main Methods
		Entity* NewEntity();
		Shader* NewShader();
		Mesh* NewMesh();
		bool RegisterEntity(Entity* poEntity);
	};

}

