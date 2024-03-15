#include "Bullet.h"
#include "HealthActor.h"

const float BULLET_LIFETIME = 10;
const float BULLET_DAMAGE = 30;

Bullet::Bullet() {
	m_fSpeed = 0;
	m_fLifetime = 0;
}

Bullet::~Bullet()
{}

void Bullet::Init(FXMVECTOR& vDir) {
	XMStoreFloat3(&m_vDirection, vDir);
	m_fSpeed = 150.f;
}

void Bullet::Update(float dT) {
	m_fLifetime += dT;
	if (m_fLifetime >= BULLET_LIFETIME) {
		m_poEntity->Kill();
		return;
	}

	XMVECTOR movement = XMLoadFloat3(&m_vDirection);
	movement *= m_fSpeed * dT;
	m_poEntity->m_poTransform->Move(&movement);
}

void Bullet::OnCollisionEnter(ave::Entity* collider) {
	m_poEntity->Kill();
	if (collider->HasComponent<HealthActor>() == false) return;
	HealthActor* healthActor = collider->GetComponent<HealthActor>();
	healthActor->TakeDamage(30);
}