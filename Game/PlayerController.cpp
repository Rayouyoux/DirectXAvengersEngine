#include "PlayerController.h"
#include "GameManager.h"

PlayerController::PlayerController() {
	m_fSpeed = 3.f;
}

PlayerController::~PlayerController() {

}

void PlayerController::Start() {
	GameManager::Get()->m_poManager->GetMainCamera();
}

void PlayerController::Update(float dT) {

}