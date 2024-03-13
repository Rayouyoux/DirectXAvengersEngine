#pragma once
#include <AvengersEngine.h>

namespace ave {
	class AvengersEngine;
	class GraphicsHandler;
	class EntityManager;
	class Entity;
}

class GameManager : public ave::Script {
private:
	static GameManager* Instance;

	ave::Entity* m_poMainCam;
	ave::Entity* m_poRotCube;
	ave::Entity* m_poSkybox;

public:
	GameManager();
	~GameManager();
	void Initialize(ave::AvengersEngine* engine);

	void Start() override;
	void Update(float dT) override;

	void InitEntities();
	void InitComponents();
	void RegisterEntities();

	static GameManager* Get() {
		return Instance;
	}

	ave::GraphicsHandler* m_poGraphics;
	ave::EntityManager* m_poManager;
};