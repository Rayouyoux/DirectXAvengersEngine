#include "pch.h"
#include "EntityManager.h"

namespace ave {

	EntityManager::EntityManager()
	{}

	EntityManager::~EntityManager(){
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			delete m_voAliveEntities[i];
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
		Entity* poEntity = new Entity();
		poEntity->Initialize();
		return poEntity;
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voAliveEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}