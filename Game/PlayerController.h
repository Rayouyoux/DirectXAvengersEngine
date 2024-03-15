#pragma once
#include <AvengersEngine.h>
#include "HealthActor.h"

class PlayerController : public ave::Script {
private:
	float m_fMoveSpeed;
	float m_fHeightSpeed;
	XMFLOAT2 m_vRotationSpeed;
	XMFLOAT3 m_vDirection;
	ave::Camera* m_poMainCamera;

	ave::EntityManager* m_poManager;
	ave::InputManager* m_poInputManager;

	POINT m_pCurrentMousePos;
	POINT m_pCenterMousePos;
	POINT m_pMouseDelta;

public:
	PlayerController();
	~PlayerController();

	void Start() override;
	void Update(float dT) override;
};

