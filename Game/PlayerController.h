#pragma once
#include <AvengersEngine.h>
#include "HealthActor.h"

class PlayerController : public ave::Script {
private:
	float m_fMoveSpeed;
	float m_fHeightSpeed;
	XMFLOAT3 m_vDirection;
	ave::Camera* m_poMainCamera;

	ave::EntityManager* m_poManager;
	ave::InputManager* m_poInputManager;

public:
	PlayerController();
	~PlayerController();

	void Start() override;
	void Update(float dT) override;
};

