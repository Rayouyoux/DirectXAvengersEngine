#pragma once
#include <AvengersEngine.h>

class PlayerController : public ave::Script {
private:
	float m_fSpeed;
	ave::Camera* m_poMainCamera;

public:
	PlayerController();
	~PlayerController();

	void Start() override;
	void Update(float dT) override;
};

