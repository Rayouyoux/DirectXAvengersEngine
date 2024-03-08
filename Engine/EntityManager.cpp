#include "pch.h"
#include "EntityManager.h"
#include "Transform.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "GraphicsHandler.h"

namespace ave {

	EntityManager::EntityManager() {
		
	}

	EntityManager::~EntityManager(){
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			delete m_voAliveEntities[i];
		}
	}

	/*void EntityManager::Init() {
		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);
	}*/

	EntityManager* EntityManager::Create() {
		return new EntityManager();
	}

	bool EntityManager::Initialize(HINSTANCE hInstance) {
		m_poGameInstance = AvengersEngine::Create();
		if (m_poGameInstance->Initialize(hInstance) == false) {
			MessageBoxA(NULL, "Euh ça bug ?", "ALED", 0);
			return false;
		}

		m_poGraphics = m_poGameInstance->GetGraphics();

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);
	}

	int EntityManager::Run() {
		return m_poGameInstance->Run();
	}

	void EntityManager::Update(float fDeltaTime) {
		/*float rot = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		m_voAliveEntities[1]->m_poTransform->RotateOnUp(rot);*/
		/*float rotCam = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		XMVECTOR vec = XMVectorSet(rotCam, 0.0f, 0.0f, 0.0f);
		m_voAliveEntities[0]->m_poTransform->Rotate(&vec);*/

		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Update(fDeltaTime);
		}
	}

	void EntityManager::LateUpdate() {
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			if (m_voAliveEntities[i]->GetIsAlive() == false) {
				Entity* EntityToDelete = m_voAliveEntities[i];
				m_voAliveEntities.erase(m_voAliveEntities.begin() + i);
				delete EntityToDelete;
			}
		}
	}

	void EntityManager::Render() {
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Render();
		}
	}

	Entity* EntityManager::GetMainCamera()
	{
		return m_poMainCamera->m_poEntity;
	}

	Entity* EntityManager::NewEntity() {
		Entity* poEntity = new Entity();
		poEntity->Initialize();
		return poEntity;
	}

	Shader* EntityManager::NewShader() {
		Shader* poShader = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera);
		return poShader;
	}

	Mesh* EntityManager::NewMesh() {
		Mesh* poMesh = new Mesh();
		poMesh->BuildBoxGeometry(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList());
		return poMesh;
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voAliveEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}