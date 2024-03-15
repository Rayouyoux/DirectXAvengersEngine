#include "Shooter.h"
#include "GameManager.h"
#include "Bullet.h"

const XMVECTOR PROJECTILE_COLOR = XMVectorSet(255, 0, 0, 1.f);

Shooter::Shooter() {
	m_poManager = nullptr;
	m_poInputManager = nullptr;

	m_fAttackRate = 5.f;
	m_fAttackDebounce = 1.f / m_fAttackRate;
}

Shooter::~Shooter()
{}

void Shooter::Start() {
	m_poManager = GameManager::Get()->m_poManager;
	m_poInputManager = m_poManager->GetInputManager();
	m_poCamera = m_poManager->GetMainCamera()->GetEntity();
}

void Shooter::Update(float dT) {
	m_fAttackDebounce += dT;
	if (m_poInputManager->GetKeyDown(ave::KeyCode::LeftMouseButton)) {
		if (m_fAttackDebounce < (1.f / m_fAttackRate)) return;
		m_fAttackDebounce = 0.f;
		SpawnBullet();
	}
}

void Shooter::SpawnBullet() {
	ave::Entity* newBullet = m_poManager->NewEntity();
	ave::MeshRenderer* renderer = newBullet->AddComponent<ave::MeshRenderer>();
	renderer->SetMesh(m_poManager->GetMesh("cylindre"));
	renderer->SetShader(m_poManager->GetShader("Color"));
	renderer->SetColor(&PROJECTILE_COLOR);
	ave::Collider* collider = newBullet->AddComponent<ave::Collider>();
	Bullet* bulletComp = newBullet->AddComponent<Bullet>();
	XMVECTOR direction = m_poCamera->m_poTransform->GetVectorDir();
	bulletComp->Init(direction);

	XMVECTOR pos = m_poCamera->m_poTransform->GetVectorPosition();
	newBullet->m_poTransform->SetVectorPosition(&pos);

	XMVECTOR scale = XMVectorSet(0.1f, 0.1f, 0.7f, 0) * 1;
	newBullet->m_poTransform->SetVectorScale(&scale);

	newBullet->m_poTransform->LookTo(&direction);

	m_poManager->RegisterEntity(newBullet);
}