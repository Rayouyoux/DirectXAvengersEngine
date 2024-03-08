#include "pch.h"
#include "EntityManager.h"
#include "Transform.h"

namespace ave {

	EntityManager::EntityManager()
	{}

	EntityManager::~EntityManager(){
		for (int i = m_voAliveEntities.size()-1; i >= 0; i--) {
			delete m_voAliveEntities[i];
		}
	}

	void EntityManager::Update(float fDeltaTime)
	{
		float rot = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		m_voAliveEntities[1]->m_poTransform->RotateOnUp(rot);
		/*float rotCam = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		XMVECTOR vec = XMVectorSet(rotCam, 0.0f, 0.0f, 0.0f);
		m_voAliveEntities[0]->m_poTransform->Rotate(&vec);*/

		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Update(fDeltaTime);
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