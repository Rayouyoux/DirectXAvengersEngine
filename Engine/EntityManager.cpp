#include "pch.h"
#include "EntityManager.h"

namespace ave {

	EntityManager::EntityManager()
	{}

	EntityManager::~EntityManager()
	{}

	void EntityManager::Start()
	{
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Start();
		}
	}

	void EntityManager::Update(float iDeltaTime)
	{
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Update(iDeltaTime);
		}
	}

	void EntityManager::LateUpdate()
	{
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			if (m_voAliveEntities[i]->GetIsAlive() == false) {
				Entity* EntityToDelete = m_voAliveEntities[i];
				m_voAliveEntities.erase(m_voAliveEntities.begin() + i);
				delete EntityToDelete;
			}
		}
	}

	void EntityManager::Render()
	{
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Render();
		}
	}

	Entity* EntityManager::NewEntity() {
		Entity* oEntity = new Entity();
		oEntity->Initialize();
		m_voAliveEntities.push_back(oEntity);
		return oEntity;
	}
}