#include "pch.h"
#include "EntityManager.h"
#include "Transform.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "GraphicsHandler.h"
#include "Texture.h"
#include "Vertex.h"

namespace ave {

	EntityManager::EntityManager() {
		
	}

	EntityManager::~EntityManager(){
		for (int i = m_voEntities.size()-1; i >= 0; i--) {
			delete m_voEntities[i];
		}
		delete m_poShader;
		delete m_poMesh;
		delete m_poTextures;
	}

	void EntityManager::Init(GraphicsHandler* poGraphics) {
		m_poGraphics = poGraphics;
		m_poTextures = new Texture();
		m_poTextures->Init(poGraphics->GetDevice());

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);

		m_poShader = NewShader();
		
		m_poMesh = NewMesh("plane");

		NewTexture("one");
	}

	/*EntityManager* EntityManager::Create() {
		return new EntityManager();
	}*/

	/*bool EntityManager::Initialize(HINSTANCE hInstance) {
		m_poGameInstance = AvengersEngine::Create();
		if (m_poGameInstance->Initialize(hInstance) == false) {
			MessageBoxA(NULL, "Euh ça bug ?", "ALED", 0);
			return false;
		}

		m_poGraphics = m_poGameInstance->GetGraphics();

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);
	}*/

	/*int EntityManager::Run() {
		return m_poGameInstance->Run();
	}*/

	void EntityManager::Update(float fDeltaTime) {
		float rot = DirectX::XMConvertToRadians(45000.0f * fDeltaTime);
		//m_voAliveEntities[1]->m_poTransform->RotateOnDir(rot);
		/*float rotCam = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		XMVECTOR vec = XMVectorSet(rotCam, 0.0f, 0.0f, 0.0f);
		m_voAliveEntities[0]->m_poTransform->Rotate(&vec);*/

		for (int i = 0; i < m_voEntities.size(); i++) {
			m_voEntities[i]->Update(fDeltaTime);
		}
	}

	void EntityManager::LateUpdate() {
		for (int i = 0; i < m_voEntities.size(); i++) {
			if (m_voEntities[i]->GetIsAlive() == false) {
				Entity* EntityToDelete = m_voEntities[i];
				m_voEntities.erase(m_voEntities.begin() + i);
				delete EntityToDelete;
			}
		}
	}

	void EntityManager::Render() {
		for (int i = 0; i < m_voEntities.size(); i++) {
			m_voEntities[i]->Render();
		}

		for (int i = 0; i < m_voEntities.size(); i++) {
			m_voEntities[i]->Render2D();
		}
	}

	Camera* EntityManager::GetMainCamera()
	{
		return m_poMainCamera;
	}

	Entity* EntityManager::NewEntity() {
		Entity* poEntity = new Entity();
		poEntity->Initialize();
		return poEntity;
	}

	Shader* EntityManager::NewShader() {
		Shader* poShader = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera, 2, m_poTextures);
		return poShader;
	}

	Mesh* EntityManager::NewMesh(std::string name) {
		/*std::string names[] = { "cube", "sphere", "cylindre", "cone", "pyramid" };
		for (int i = 0; i < names->length(); i++) {
			Mesh* poMesh = new Mesh();
			poMesh->BuildBoxGeometry<VERTEX_UV>(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList(), name);
		}*/
		Mesh* poMesh = new Mesh();
		poMesh->BuildBoxGeometry<VERTEX_UV>(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList(), name);
		return poMesh;
	}

	void EntityManager::NewTexture(std::string name) {
		m_poTextures->LoadTexture(name, L"..\\Engine\\Textures\\" + std::wstring(name.begin(), name.end()) + L".dds");
		m_poTextures->BuildDescriptorHeaps(name, m_poGraphics->GetCbvDescriptor());
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}