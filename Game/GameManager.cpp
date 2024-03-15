#include "GameManager.h"
#include "PlayerController.h"
#include "HealthActor.h"
#include "Shooter.h"
#include "AsteroidSpawner.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;

	m_poMainCam = nullptr;
	m_poPlayer = nullptr;
	m_poSkybox = nullptr;
	m_poAsteroidSpawner = nullptr;

	m_poDestructionBehaviour = nullptr;

	if (GameManager::Instance) return;
	GameManager::Instance = this;
}

GameManager::~GameManager() {
	delete m_poDestructionBehaviour;
}

void GameManager::Initialize(ave::AvengersEngine* engine) {
	m_poGraphics = engine->GetGraphics();
	m_poManager = engine->GetManager();

	m_poDestructionBehaviour = new ave::Particles::ParticleBehaviour();
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
	m_poManager->NewTexture("blue_particle", "Textures/blue_particle.dds");

	// Particles //
	m_poDestructionBehaviour->EmissionType = ave::Particles::Burst;
	m_poDestructionBehaviour->BurstAmount = 50;
	m_poDestructionBehaviour->Size = 3;
	m_poDestructionBehaviour->EndSize = 0;
	m_poDestructionBehaviour->SizeOverTime = true;
	m_poDestructionBehaviour->SizeVariation = 0.4f;
	m_poDestructionBehaviour->EndSizeVariation = 0.4f;
	m_poDestructionBehaviour->Speed = 6;
	m_poDestructionBehaviour->SpeedVariation = 0.4f;
	m_poDestructionBehaviour->EndSpeed = 0;
	m_poDestructionBehaviour->EndSpeedVariation = 0.4f;
	m_poDestructionBehaviour->SpeedOverTime = true;
	m_poDestructionBehaviour->MaxLifetime = 1.4f;
	m_poDestructionBehaviour->LifetimeVariation = 0.6f;
}

void GameManager::InitEntities() {
	m_poMainCam = m_poManager->GetMainCamera()->GetEntity();
	m_poPlayer = m_poManager->NewEntity();
	m_poSkybox = m_poManager->NewEntity();
	m_poAsteroidSpawner = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	// Player
	m_poPlayer->AddComponent<PlayerController>();
	HealthActor* healthActor = m_poPlayer->AddComponent<HealthActor>();
	healthActor->SetMaxHealth(100);
	m_poPlayer->AddComponent<Shooter>();

	// Skybox
	ave::MeshRenderer* skyboxRenderer = m_poSkybox->AddComponent<ave::MeshRenderer>();
	skyboxRenderer->SetMesh(m_poManager->GetMesh("skyboxTexture"));
	skyboxRenderer->SetShader(m_poManager->GetShader("Texture"));
	skyboxRenderer->SetTexture(m_poManager->GetTexture("nebula"));

	DirectX::XMVECTOR skyboxScale = XMVectorSet(1.0f, 1.0f, 1.f, 0.f) * 500;
	m_poSkybox->m_poTransform->Scale(&skyboxScale);

	// Asteroid Spawner
	AsteroidSpawner* spawner = m_poAsteroidSpawner->AddComponent<AsteroidSpawner>();
	spawner->SetBehaviour(m_poDestructionBehaviour);
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poPlayer);
	m_poManager->RegisterEntity(m_poSkybox);
	m_poManager->RegisterEntity(m_poAsteroidSpawner);
}

void GameManager::Update(float dT) {
}