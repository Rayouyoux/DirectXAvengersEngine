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
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			delete m_voAliveEntities[i];
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
		
		m_poMesh = NewMesh("cube");

		NewTexture("image");
	}

	/*EntityManager* EntityManager::Create() {
		return new EntityManager();
	}*/

	/*bool EntityManager::Initialize(HINSTANCE hInstance) {
		m_poGameInstance = AvengersEngine::Create();
		if (m_poGameInstance->Initialize(hInstance) == false) {
			MessageBoxA(NULL, "Euh �a bug ?", "ALED", 0);
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
		float rot = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		m_voAliveEntities[1]->m_poTransform->RotateOnUp(rot);
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
		m_voAliveEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}