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
	}

	void EntityManager::Init(GraphicsHandler* poGraphics) {
		m_poGraphics = poGraphics;
		m_poInput = new InputManager();
		m_poInput->Init(poGraphics->GetWindow());

		Entity* poCamera = NewEntity();
		m_poMainCamera = poCamera->AddComponent<Camera>();
		RegisterEntity(poCamera);

		CreateShader();
		CreateMesh();
	}

	void EntityManager::Update(float fDeltaTime) {
		m_poInput->UpdateKeyStates();

		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Update(fDeltaTime);
		}
	}

	void EntityManager::LateUpdate(float dT) {
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->LateUpdate(dT);
		}

		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->LateUpdate(dT);
			if (m_voAliveEntities[i]->GetIsAlive() == false) {
				Entity* entityToDelete = m_voAliveEntities[i];
				entityToDelete->OnDestroy();
				m_voAliveEntities.erase(m_voAliveEntities.begin() + i);
				delete entityToDelete;
			}
		}
	}

	void EntityManager::Render() {
		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Render();
		}

		for (int i = 0; i < m_voAliveEntities.size(); i++) {
			m_voAliveEntities[i]->Render2D();
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
		poShaderTexture->CreateShader(m_poGraphics, m_poMainCamera, 2);
		m_poShaders.insert(std::pair<std::string, Shader*>("Texture", poShaderTexture));
	} 

	void EntityManager::CreateMesh() {
		std::string names[] = { "cube", "sphere", "cylindre", "cone", "pyramid", "skybox"};
		for (int i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
			Mesh* poMesh = new Mesh();
			XMVECTOR oColor = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
			//FXMVECTOR oColor = {1.0f, 1.0f, 1.0f, 1.0f};
			poMesh->BuildBoxGeometry<VERTEX_COLOR>(m_poGraphics->GetDevice(), m_poGraphics->GetCommandList(), names[i], &oColor);
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

		Texture* poTexture = new Texture();
		m_poTextures.insert(std::make_pair(name, poTexture));
		poTexture->Init(m_poGraphics->GetDevice());
		poTexture->LoadTexture(name,std::wstring(filename.begin(), filename.end()), m_poGraphics->GetCbvDescriptor(),m_poGraphics);
		poTexture->BuildSrvDesc(m_poGraphics->GetCbvDescriptor(), m_poTextures.size());
	}

	bool EntityManager::RegisterEntity(Entity* poEntity) {
		m_voAliveEntities.push_back(poEntity);
		poEntity->Start();
		return true;
	}
}