#include "Debris.h"
#include "GameManager.h"

Debris::Debris() {
}

Debris::~Debris()
{}

void Debris::Init(float fMaxLifetime) {
	m_fMaxLifetime = fMaxLifetime;
}

void Debris::Start() {
	m_poManager = GameManager::Get()->m_poManager;
}

void Debris::Update(float dT) {
	m_fLifetime += dT;
	if (m_fLifetime < m_fMaxLifetime) return;
	m_poEntity->Kill();
}
