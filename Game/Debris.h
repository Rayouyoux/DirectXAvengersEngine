#pragma once
#include <AvengersEngine.h>

class Debris : public ave::Script {
protected:
	float m_fLifetime;
	float m_fMaxLifetime;

	ave::EntityManager* m_poManager;

public:
	Debris();
	~Debris();

	void Init(float fMaxLifetime);

	void Start() override;
	void Update(float dT) override;
};

