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
		for (int i = m_voAliveEntities.size()-1; i >= 0; i--) {
			delete m_voAliveEntities[i];
		}
		delete m_poTextures;
	}

	void EntityManager::Init(GraphicsHandler* poGraphics) {
		m_poGraphics = poGraphics;
		m_poInput = new InputManager();
		m_poInput->Init(poGraphics->GetWindow());
		m_poTextures = new Texture();
		m_poTextures->Init(poGraphics->GetDevice());

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);

		CreateShader();
		CreateMesh();

		/*NewTexture("victor", "..\\Engine\\Textures\\image.dds");*/
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
		float rot = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		m_voAliveEntities[1]->m_poTransform->RotateOnUp(rot);
		/*float rotCam = DirectX::XMConvertToRadians(45.0f * fDeltaTime);
		XMVECTOR vec = XMVectorSet(rotCam, 0.0f, 0.0f, 0.0f);
		m_voAliveEntities[0]->m_poTransform->Rotate(&vec);*/

		m_poInput->UpdateKeyStates();

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

	void EntityManager::CreateShader() {
		Shader* poShader = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera, 1);
		m_poShaders.insert(std::pair<std::string, Shader*>("Color", poShader));

		Shader* poShaderTexture = new Shader();
		poShader->CreateShader(m_poGraphics, m_poMainCamera, 2, m_poTextures);
		m_poShaders.insert(std::pair<std::string, Shader*>("Texture", poShader));
	}

	void EntityManager::CreateMesh() {
		std::string names[] = { "cube", "sphere", "cylindre", "cone", "pyramid" };
		for (int i = 0; i < names->length(); i++) {
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

	void EntityManager::NewTexture(std::string name, std::string filename) {
		m_poTextures->LoadTexture(name, std::wstring(filename.begin(), filename.end()));
		m_poTextures->BuildDescriptorHeaps(name, m_poGraphics->GetCbvDescriptor());
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voAliveEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}