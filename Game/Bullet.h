#pragma once
#include <AvengersEngine.h>

class Bullet : public ave::Script {
protected:
	XMFLOAT3 m_vDirection;
	float m_fSpeed;
	float m_fLifetime;

public:
	Bullet();
	~Bullet();

	void Init(FXMVECTOR& direction);
	void Update(float dT) override;

	void OnCollisionEnter(ave::Entity* collider);
};