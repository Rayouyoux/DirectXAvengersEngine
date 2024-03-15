#include "PlayerController.h"
#include "GameManager.h"

using namespace ave;

PlayerController::PlayerController() {
	m_fMoveSpeed = 3.f;
	m_fHeightSpeed = 2.f;
}

PlayerController::~PlayerController() {

}

void PlayerController::Start() {
	m_poManager = GameManager::Get()->m_poManager;
	m_poInputManager = m_poManager->GetInputManager();
	m_poMainCamera = m_poManager->GetMainCamera();
}

void PlayerController::Update(float dT) {
	m_vDirection = { 0, 0, 0 };
	if (m_poInputManager->GetKeyDown(KeyCode::Space)) {
		m_vDirection.y += 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::LeftControl)) {
		m_vDirection.y -= 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::Q)) {
		m_vDirection.x -= 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::D)) {
		m_vDirection.x += 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::Z)) {
		m_vDirection.z += 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::S)) {
		m_vDirection.z -= 1;
	}

	XMVECTOR movement = XMVectorSet(0, 0, 0, 0);
	if (m_vDirection.y != 0) {
		movement += XMVectorSet(0, m_vDirection.y, 0, 0) * m_fHeightSpeed;
		m_vDirection.y = 0;
	}

	XMVECTOR dir = XMLoadFloat3(&m_vDirection);
	dir = XMVector3Normalize(dir);
	movement += dir * m_fMoveSpeed;

	movement *= dT;
	m_poEntity->m_poTransform->Move(&movement);
}