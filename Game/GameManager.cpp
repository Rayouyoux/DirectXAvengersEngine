#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;
}

GameManager::~GameManager() {
	delete GameManager::Instance;
}

void GameManager::Initialize(ave::AvengersEngine& engine) {
	m_poGraphics = engine.AccessGraphics();
	m_poManager = engine.AccessEntityManager();
}

void GameManager::Start() {
	InitEntities();
	InitComponents();
	RegisterEntities();
}

void GameManager::InitEntities() {
	m_poRotCube = m_poManager->NewEntity();
}

void GameManager::InitComponents() {
	ave::MeshRenderer* renderer = m_poRotCube->AddComponent<ave::MeshRenderer>();
}

void GameManager::RegisterEntities() {
	m_poManager->RegisterEntity(m_poRotCube);
}

void GameManager::Update(float dT) {

}