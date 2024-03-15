#pragma once
#include <AvengersEngine.h>

class Shooter : public ave::Script {
protected:
	ave::EntityManager* m_poManager;
	ave::InputManager* m_poInputManager;

	float m_fAttackRate;
	float m_fAttackDebounce;

public:
	Shooter();
	~Shooter();

	void Start() override;
	void Update(float dT) override;

	void SpawnBullet();
};