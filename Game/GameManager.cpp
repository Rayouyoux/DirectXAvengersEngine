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
	m_poManager->NewTexture("image", "../Engine/Textures/image.dds");
	m_poManager->NewTexture("default_particle", "../Engine/Textures/default_particle.dds");
	m_poManager->NewTexture("bricks", "../Engine/Textures/bricks.dds");
	m_poManager->NewTexture("blue_particle", "Textures/blue_particle.dds");
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

	// Camera
	DirectX::XMVECTOR camPos = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
	m_poMainCam->m_poTransform->SetVectorPosition(&camPos);

	DirectX::XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	m_poMainCam->m_poTransform->LookTo(&direction);

	DirectX::XMVECTOR posCube = XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);
	m_poRotCube->m_poTransform->SetVectorPosition(&posCube);

	ave::MeshRenderer* cubeRenderer = m_poRotCube->AddComponent<ave::MeshRenderer>();
	cubeRenderer->SetMesh(m_poManager->GetMesh("cubeTexture"));
	cubeRenderer->SetShader(m_poManager->GetShader("Texture"));
	cubeRenderer->SetTexture(m_poManager->GetTexture("image"));

	//ave::MeshRenderer* skyboxRenderer = m_poSkybox->AddComponent<ave::MeshRenderer>();
	//skyboxRenderer->SetMesh(m_poManager->GetMesh("skyboxTexture"));
	//skyboxRenderer->SetShader(m_poManager->GetShader("Texture"));
	//skyboxRenderer->SetTexture(m_poManager->GetTexture("image"));

	//DirectX::XMVECTOR skyboxScale = XMVectorSet(1.0f, 1.0f, 1.f, 0.f) * 100;
	//m_poSkybox->m_poTransform->Scale(&skyboxScale);

	m_poDefaultBehaviour = new ave::Particles::ParticleBehaviour();
	m_poDefaultBehaviour->EmissionType = ave::Particles::ParticleEmissionType::Burst;
	m_poDefaultBehaviour->BurstAmount = 60;
	m_poDefaultBehaviour->Size = .4f;
	m_poDefaultBehaviour->SizeOverTime = true;
	m_poDefaultBehaviour->EndSize = 0;
	m_poDefaultBehaviour->Scale = XMFLOAT3(1, 1, 1);
	m_poDefaultBehaviour->ScaleOverTime = false;
	m_poDefaultBehaviour->RotateOverTime = false;
	m_poDefaultBehaviour->RotSpeed = 0;
	m_poDefaultBehaviour->SpeedOverTime = true;
	m_poDefaultBehaviour->Speed = 3.f;
	m_poDefaultBehaviour->EndSpeed = 0.f;
	m_poDefaultBehaviour->MaxLifetime = 0.5f;
	m_poDefaultBehaviour->SpeedVariation = 0.4f;
	m_poDefaultBehaviour->LifetimeVariation = 1;
	m_poDefaultBehaviour->SizeVariation = .7;

	ave::Particles::ParticleSystem* particleSystem = m_poParticleSystemEx->AddComponent<ave::Particles::ParticleSystem>();
	particleSystem->SetBehaviour(m_poDefaultBehaviour);
	particleSystem->Initialize(m_poGraphics);
	particleSystem->SetShader(m_poManager->GetShader("Texture"));
	particleSystem->SetTexture(m_poManager->GetTexture("blue_particle"));
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poPlayer);
	//m_poManager->RegisterEntity(m_poRotCube);
	m_poManager->RegisterEntity(m_poSkybox);
	m_poManager->RegisterEntity(m_poParticleSystemEx);
}

void GameManager::Update(float dT) {

}