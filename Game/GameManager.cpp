#include "GameManager.h"
#include "PlayerController.h"
#include "HealthActor.h"
#include "Collider.h"

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
	delete m_poDefaultBehaviour;
}

void GameManager::Initialize(ave::AvengersEngine* engine) {
	m_poGraphics = engine->GetGraphics();
	m_poManager = engine->GetManager();
}

void GameManager::Start() {
	InitResources();
	InitEntities();
	InitComponents();
	RegisterEntities();
}

void GameManager::InitResources() {
	m_poManager->NewTexture("nebula", "Textures/seamless_nebula.dds");
	m_poManager->NewTexture("moon_rock", "Textures/moon_rock.dds");
}

void GameManager::InitEntities() {
	m_poMainCam = m_poManager->GetMainCamera()->GetEntity();
	m_poPlayer = m_poManager->NewEntity();
	m_poRotCube = m_poManager->NewEntity();
	m_poSkybox = m_poManager->NewEntity();
	m_poParticleSystemEx = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	// Player
	PlayerController* controller = m_poPlayer->AddComponent<PlayerController>();
	HealthActor* healthActor = m_poPlayer->AddComponent<HealthActor>();
	healthActor->SetMaxHealth(100);

	//DirectX::XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	//m_poMainCam->m_poTransform->LookTo(&direction);

	DirectX::XMVECTOR posCube = XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);
	m_poRotCube->m_poTransform->SetVectorPosition(&posCube);

	// Skybox
	ave::MeshRenderer* skyboxRenderer = m_poSkybox->AddComponent<ave::MeshRenderer>();
	skyboxRenderer->SetMesh(m_poManager->GetMesh("skyboxTexture"));
	skyboxRenderer->SetShader(m_poManager->GetShader("Texture"));
	skyboxRenderer->SetTexture(m_poManager->GetTexture("nebula"));

	DirectX::XMVECTOR skyboxScale = XMVectorSet(1.0f, 1.0f, 1.f, 0.f) * 500;
	m_poSkybox->m_poTransform->Scale(&skyboxScale);

	// Cube
	ave::MeshRenderer* meshRenderer = m_poRotCube->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh(m_poManager->GetMesh("sphereTexture"));
	meshRenderer->SetShader(m_poManager->GetShader("Texture"));
	meshRenderer->SetTexture(m_poManager->GetTexture("moon_rock"));

	ave::MeshRenderer* playerRenderer = m_poPlayer->AddComponent<MeshRenderer>();
	playerRenderer->SetMesh(m_poManager->GetMesh("cubeTexture"));
	playerRenderer->SetShader(m_poManager->GetShader("Texture"));
	playerRenderer->SetTexture(m_poManager->GetTexture("moon_rock"));
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poPlayer);
	m_poManager->RegisterEntity(m_poRotCube);
	m_poManager->RegisterEntity(m_poSkybox);
	m_poManager->RegisterEntity(m_poParticleSystemEx);
}

void GameManager::Update(float dT) {
	XMVECTOR offset = XMVectorSet(1, 0, 0, 0) * dT;
	m_poParticleSystemEx->m_poTransform->Move(&offset);
}