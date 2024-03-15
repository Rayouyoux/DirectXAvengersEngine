#include "AsteroidSpawner.h"
#include "GameManager.h"
#include "Asteroid.h"
#include "HealthActor.h"

using namespace ave;

AsteroidSpawner::AsteroidSpawner() {
	m_poManager = nullptr;
	m_fSpawnRate = 0.7f;
	m_fDebounce = 1 / m_fSpawnRate;
}

AsteroidSpawner::~AsteroidSpawner()
{}

void AsteroidSpawner::Start() {
	m_poManager = GameManager::Get()->m_poManager;
}

void AsteroidSpawner::Update(float dT) {
	m_fDebounce += dT;
	if (m_fDebounce < (1 / m_fSpawnRate)) return;
	m_fDebounce = 0;
	SpawnNewAsteroid();
	m_fSpawnRate *= 1.01f;
}

void AsteroidSpawner::SpawnNewAsteroid() {
	Entity* newAst = m_poManager->NewEntity();
	MeshRenderer* renderer = newAst->AddComponent<MeshRenderer>();
	renderer->SetShader(m_poManager->GetShader("Texture"));
	renderer->SetMesh(m_poManager->GetMesh("sphereTexture"));
	renderer->SetTexture(m_poManager->GetTexture("moon_rock"));
	newAst->AddComponent<Collider>();
	HealthActor* healthActor = newAst->AddComponent<HealthActor>();
	healthActor->SetMaxHealth(30);
	Asteroid* asteroidComp = newAst->AddComponent<Asteroid>();
	asteroidComp->Init();
	asteroidComp->SetBehaviour(m_poBehaviour);

	XMVECTOR pos = XMVectorSet(
		Maths::Random::Range(-240, 240),
		Maths::Random::Range(-240, 240),
		Maths::Random::Range(-240, 240),
		0
	);
	newAst->m_poTransform->SetVectorPosition(&pos);

	m_poManager->RegisterEntity(newAst);
}