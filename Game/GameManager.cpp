#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;
}

GameManager::~GameManager() {
	delete GameManager::Instance;
}

void GameManager::Initialize(ave::AvengersEngine* engine) {
	m_poGraphics = engine->GetGraphics();
	m_poManager = engine->GetManager();
}

void GameManager::Start() {
	InitEntities();
	InitComponents();
	RegisterEntities();
}

void GameManager::InitEntities() {
	m_poMainCam = m_poManager->GetMainCamera()->GetEntity();
	m_poRotCube = m_poManager->NewEntity();
	m_poSkybox = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	DirectX::XMVECTOR posCube = XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);
	m_poRotCube->m_poTransform->SetVectorPosition(&posCube);
	
	DirectX::XMVECTOR camPos = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
	m_poMainCam->m_poTransform->SetVectorPosition(&camPos);

	DirectX::XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	m_poMainCam->m_poTransform->LookTo(&direction);

	ave::MeshRenderer* cubeRenderer = m_poRotCube->AddComponent<ave::MeshRenderer>();
	cubeRenderer->SetMesh(m_poManager->GetMesh("cubeTexture"));
	cubeRenderer->SetShader(m_poManager->GetShader("Texture"));
	cubeRenderer->SetTexture(m_poManager->GetTexture("image"));

	ave::MeshRenderer* skyboxRenderer = m_poSkybox->AddComponent<ave::MeshRenderer>();
	skyboxRenderer->SetMesh(m_poManager->GetMesh("skyboxTexture"));
	skyboxRenderer->SetShader(m_poManager->GetShader("Texture"));
	skyboxRenderer->SetTexture(m_poManager->GetTexture("image"));

	DirectX::XMVECTOR skyboxScale = XMVectorSet(1.0f, 1.0f, 1.f, 0.f) * 100;
	m_poSkybox->m_poTransform->Scale(&skyboxScale);
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poRotCube);
	m_poManager->RegisterEntity(m_poSkybox);
}

void GameManager::Update(float dT) {

}