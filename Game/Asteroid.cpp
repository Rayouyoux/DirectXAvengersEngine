#include "Asteroid.h"
#include "GameManager.h"
#include "HealthActor.h"
#include "PlayerController.h"
#include "Debris.h"

using namespace ave;

#define ASTEROID_MAX_LIFETIME 15

Asteroid::Asteroid() {
	m_fBaseSpeed = 100.f;
	m_fSpeed = 0;
	m_fDamage = 20;
	m_fLifetime = 0;
	m_fSize = 3;
}

Asteroid::~Asteroid()
{}

void Asteroid::Init() {
	m_fSpeed = m_fBaseSpeed * (1 - Maths::Random::Range(-.4f, .4f));
	
	float x = Maths::Random::Range(-360, 360);
	float y = Maths::Random::Range(-360, 360);
	float z = Maths::Random::Range(-360, 360);
	XMVECTOR randomDir = XMVectorSet(x, y, z, 0);
	randomDir = XMVector3Normalize(randomDir);

	XMStoreFloat3(&m_vDirection, randomDir);

	XMVECTOR size = XMVectorSet(
		m_fSize * (1 + Maths::Random::Range(-0.3f, 0.3f)),
		m_fSize * (1 + Maths::Random::Range(-0.3f, 0.3f)),
		m_fSize * (1 + Maths::Random::Range(-0.3f, 0.3f)),
		0
	);
	m_poEntity->m_poTransform->SetVectorScale(&size);
}

void Asteroid::SetBehaviour(Particles::ParticleBehaviour* poBehaviour) {
	m_poBehaviour = poBehaviour;
}

void Asteroid::Start() {
	m_poManager = GameManager::Get()->m_poManager;
}

void Asteroid::Update(float dT) {
	m_fLifetime += dT;
	if (m_fLifetime > ASTEROID_MAX_LIFETIME) {
		m_poEntity->Kill();
		return;
	}

	XMVECTOR movement = XMLoadFloat3(&m_vDirection);
	movement *= m_fSpeed * dT;
	m_poEntity->m_poTransform->Move(&movement);

	XMVECTOR rotation = XMLoadFloat3(&m_vDirection);
	rotation *= -1 * dT;
	m_poEntity->m_poTransform->Rotate(&rotation);
}

void Asteroid::OnCollisionEnter(ave::Entity* collider) {
	if (collider->HasComponent<Asteroid>()) {
		m_poEntity->Kill();
	}

	if (collider->HasComponent<PlayerController>() == false) return;
	m_poEntity->Kill();
	HealthActor* healthActor = collider->GetComponent<HealthActor>();
	healthActor->TakeDamage(m_fDamage);
}

void Asteroid::OnDestroy() {
	Entity* blowParticles = m_poManager->NewEntity();
	ave::Particles::ParticleSystem* particles = blowParticles->AddComponent<ave::Particles::ParticleSystem>();
	particles->SetBehaviour(m_poBehaviour);
	particles->Initialize(GameManager::Get()->m_poGraphics);
	particles->SetShader(m_poManager->GetShader("Texture"));
	particles->SetTexture(m_poManager->GetTexture("blue_particle"));
	Debris* debris = blowParticles->AddComponent<Debris>();
	debris->Init(5);

	XMVECTOR pos = m_poEntity->m_poTransform->GetVectorPosition();
	blowParticles->m_poTransform->SetVectorPosition(&pos);

	m_poManager->RegisterEntity(blowParticles);

	particles->Burst();
}