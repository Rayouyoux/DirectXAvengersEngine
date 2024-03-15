#include "Shooter.h"
#include "GameManager.h"

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
}