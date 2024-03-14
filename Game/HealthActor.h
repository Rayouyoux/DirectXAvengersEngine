#pragma once
#include <AvengersEngine.h>

using namespace ave;

class HealthActor : public Script {
private:
	float m_fMaxHealth;
	float m_fHealth;

public:
	HealthActor();
	~HealthActor();

	void Start() override;

	/// <summary>
	/// Deals n amount of damage to this health actor.
	/// </summary>
	/// <param name="damage">The amount of damage to deal.</param>
	/// <returns>True if health is lower than 0.</returns>
	bool TakeDamage(float damage);

	void SetMaxHealth(float fMaxHealth) { m_fMaxHealth = fMaxHealth; }

	float GetMaxHealth() { return m_fMaxHealth; }
	float GetHealth() { return m_fHealth; }
};

