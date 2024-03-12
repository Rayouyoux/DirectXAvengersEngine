#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;
}

GameManager::~GameManager() {
	delete GameManager::Instance;
}

void GameManager::Initialize(ave::AvengersEngine& engine) {
	m_poGraphics = engine.AccessGraphics();
	m_poManager = engine.AccessEntityManager();
}

void GameManager::Start() {
	InitEntities();
	InitComponents();
	RegisterEntities();
}

void GameManager::InitEntities() {
	m_poMainCam = m_poManager->GetMainCamera()->GetEntity();
	m_poRotCube = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	XMVECTOR posCube = XMVectorSet(5.0f, 0.0f, 0.0f, 0.0f);
	m_poRotCube->m_poTransform->SetVectorPosition(&posCube);

	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
	m_poMainCam->m_poTransform->SetVectorPosition(&pos);

	XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	m_poMainCam->m_poTransform->LookTo(&direction);

	ave::MeshRenderer* renderer = m_poRotCube->AddComponent<ave::MeshRenderer>();
	renderer->SetMesh(m_poManager->GetMesh());
	renderer->SetShader(m_poManager->GetShader());
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poRotCube);
}

void GameManager::Update(float dT) {

}