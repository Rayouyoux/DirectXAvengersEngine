#include "GameManager.h"
#include "PlayerController.h"
#include "HealthActor.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;

	m_poMainCam = nullptr;
	m_poPlayer = nullptr;
	m_poRotCube = nullptr;
	m_poSkybox = nullptr;

	if (GameManager::Instance) return;
	GameManager::Instance = this;
}

GameManager::~GameManager() {

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
	m_poPlayer = m_poManager->NewEntity();
	m_poRotCube = m_poManager->NewEntity();
	m_poSkybox = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	// Player
	PlayerController* controller = m_poPlayer->AddComponent<PlayerController>();
	HealthActor* healthActor = m_poPlayer->AddComponent<HealthActor>();
	healthActor->SetMaxHealth(100);
	MeshRenderer* playerRenderer = m_poPlayer->AddComponent<MeshRenderer>();
	playerRenderer->SetMesh(m_poManager->GetMesh("cubeTexture"));
	playerRenderer->SetShader(m_poManager->GetShader("Texture"));
	playerRenderer->SetTexture(m_poManager->GetTexture("image"));

	// Camera
	DirectX::XMVECTOR camPos = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
	m_poMainCam->m_poTransform->SetVectorPosition(&camPos);

	DirectX::XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	m_poMainCam->m_poTransform->LookTo(&direction);

	DirectX::XMVECTOR posCube = XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);
	m_poRotCube->m_poTransform->SetVectorPosition(&posCube);

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
	m_poManager->RegisterEntity(m_poPlayer);
	m_poManager->RegisterEntity(m_poRotCube);
	m_poManager->RegisterEntity(m_poSkybox);
}

void GameManager::Update(float dT) {

}