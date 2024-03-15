#include "PlayerController.h"
#include "GameManager.h"

using namespace ave;

PlayerController::PlayerController() {
	m_fMoveSpeed = 30.f;
	m_fHeightSpeed = 20.f;
	m_pCenterMousePos = { 0, 0 };
	m_pCurrentMousePos = { 0, 0 };
	m_pMouseDelta = { 0, 0 };
	m_vRotationSpeed = { 0.001f, 0.0007f };
}

PlayerController::~PlayerController() {

}

void PlayerController::Start() {
	m_poManager = GameManager::Get()->m_poManager;
	m_poInputManager = m_poManager->GetInputManager();
	m_poMainCamera = m_poManager->GetMainCamera();

	int width = GameManager::Get()->m_poGraphics->GetWindowWidth() * 0.5f;
	int height = GameManager::Get()->m_poGraphics->GetWindowHeight() * 0.5f;
	m_pCenterMousePos = { width, height };
	m_pCurrentMousePos = { width, height };
	ShowCursor(false);
}

void PlayerController::Update(float dT) {
	m_pCurrentMousePos = m_poInputManager->MousePos();
	m_pMouseDelta = {
		m_pCurrentMousePos.x - m_pCenterMousePos.x,
		m_pCurrentMousePos.y - m_pCenterMousePos.y
	};

	m_vDirection = { 0, 0, 0 };
	if (m_poInputManager->GetKeyDown(KeyCode::Space)) {
		m_vDirection.y -= 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::LeftControl)) {
		m_vDirection.y += 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::Q)) {
		m_vDirection.x += 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::D)) {
		m_vDirection.x -= 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::Z)) {
		m_vDirection.z -= 1;
	}
	if (m_poInputManager->GetKeyDown(KeyCode::S)) {
		m_vDirection.z += 1;
	}

	m_poMainCamera->GetEntity()->m_poTransform->Rotate(-m_pMouseDelta.x * m_vRotationSpeed.x,
		m_pMouseDelta.y * m_vRotationSpeed.y, 0);

	XMVECTOR movement = XMVectorSet(0, 0, 0, 0);
	if (m_vDirection.y != 0) {
		movement += XMVectorSet(0, m_vDirection.y, 0, 0) * m_fHeightSpeed;
		m_vDirection.y = 0;
	}

	XMVECTOR dir = XMLoadFloat3(&m_vDirection);
	dir = XMVector3Normalize(dir);
	movement += dir * m_fMoveSpeed;

	movement *= dT;
	m_poMainCamera->GetEntity()->m_poTransform->Move(&movement);

	int width = GameManager::Get()->m_poGraphics->GetWindowWidth() * 0.5f;
	int height = GameManager::Get()->m_poGraphics->GetWindowHeight() * 0.5f;
	SetCursorPos(width, height);
	m_pCenterMousePos.x = width;
	m_pCenterMousePos.y = height;
	HWND window = GameManager::Get()->m_poGraphics->GetWindow();
	ScreenToClient(window, &m_pCenterMousePos);
}