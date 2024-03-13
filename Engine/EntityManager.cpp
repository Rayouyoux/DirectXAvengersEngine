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
#include "InputManager.h"

namespace ave {

	EntityManager::EntityManager() {
		
	}

	EntityManager::~EntityManager(){
		for (int i = m_voEntities.size()-1; i >= 0; i--) {
			delete m_voEntities[i];
		}
	}

	void EntityManager::Init(GraphicsHandler* poGraphics) {
		m_poGraphics = poGraphics;
		m_poInput = new InputManager();
		/*m_poInput->Init(poGraphics->GetWindow());*/

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);

		CreateShader();
		CreateMesh();

		/*NewTexture("victor", "..\\Engine\\Textures\\image.dds");*/
		NewTexture("bricks");
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


		/*m_poInput->UpdateKeyStates();*/
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

	void EntityManager::CreateShader() {
		Shader* poShader = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera, 1);
		m_poShaders.insert(std::pair<std::string, Shader*>("Color", poShader));

		Shader* poShaderTexture = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera, 2);
		m_poShaders.insert(std::pair<std::string, Shader*>("Texture", poShader));
	}

	void EntityManager::CreateMesh() {
		std::string names[] = { "cube", "sphere", "cylindre", "cone", "pyramid", "skybox"};
		for (int i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
			Mesh* poMesh = new Mesh();
			poMesh->BuildBoxGeometry<VERTEX_COLOR>(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList(), names[i]);
			m_poMeshs.insert(std::pair<std::string, Mesh*>(names[i], poMesh));

			Mesh* poMeshTexture = new Mesh();
			poMeshTexture->BuildBoxGeometry<VERTEX_UV>(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList(), names[i]);
			m_poMeshs.insert(std::pair<std::string, Mesh*>(names[i] + "Texture", poMeshTexture));
		}
	}

	Mesh* EntityManager::GetMesh(std::string name) {
		return m_poMeshs.find(name)->second;
	}

	Shader* EntityManager::GetShader(std::string name) {
		return m_poShaders.find(name)->second;
	}

	void EntityManager::NewTexture(std::string name) {

		Texture* poTexture = new Texture();
		m_poTextures.insert(std::make_pair(name, poTexture));
		poTexture->Init(m_poGraphics->GetDevice());
		poTexture->LoadTexture(name, L"..\\Engine\\Textures\\" + std::wstring(name.begin(), name.end()) + L".dds", m_poGraphics->GetCbvDescriptor());
		poTexture->BuildSrvDesc(m_poGraphics->GetCbvDescriptor(), m_poTextures.size());
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}