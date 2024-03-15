#pragma once
#include <AvengersEngine.h>

class Asteroid : public ave::Script {
protected:
	XMFLOAT3 m_vDirection;
	float m_fBaseSpeed;
	float m_fSpeed;
	float m_fDamage;
	float m_fLifetime;
	float m_fSize;

	ave::EntityManager* m_poManager;
	ave::Particles::ParticleBehaviour* m_poBehaviour;

public:
	Asteroid();
	~Asteroid();

	void Init();
	
	void SetBehaviour(ave::Particles::ParticleBehaviour* poBehaviour);

	void Start() override;
	void Update(float dT) override;

	void OnCollisionEnter(ave::Entity* collider) override;
	void OnDestroy() override;
};