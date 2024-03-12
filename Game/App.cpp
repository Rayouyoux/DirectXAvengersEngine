#include "App.h"

App* App::Instance = nullptr;

App::App() {
	m_poGraphics = nullptr;
	m_poManager = nullptr;
}

App::~App() {
	delete App::Instance;
}

bool App::Create() {
	if (App::Instance != nullptr) return false;

	App::Instance = new App();
	return true;
}

void App::Initialize(ave::AvengersEngine& engine) {
	m_poGraphics = engine.AccessGraphics();
	m_poManager = engine.AccessEntityManager();
}

void App::Start() {
	InitEntities();
}

void App::InitEntities() {

}

void App::InitComponents() {

}

void App::Update(float dT) {

}