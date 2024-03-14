#include "HealthActor.h"

HealthActor::HealthActor() {
    m_fHealth = 0;
}

HealthActor::~HealthActor() {

}

void HealthActor::Start() {
    m_fHealth = m_fMaxHealth;
}

bool HealthActor::TakeDamage(float damage)
{
    m_fHealth -= damage;

    return m_fHealth <= 0;
}
