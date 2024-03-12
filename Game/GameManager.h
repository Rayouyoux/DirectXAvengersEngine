#pragma once
#include <AvengersEngine.h>

class GameManager : public ave::Script {
private:
	static GameManager* Instance;
	ave::GraphicsHandler* m_poGraphics;
	ave::EntityManager* m_poManager;

	ave::Entity* m_poRotCube;

private:
	GameManager();
	~GameManager();

public:
	void Initialize(ave::AvengersEngine& engine);

	void Start() override;
	void Update(float dT) override;

	void InitEntities();
	void InitComponents();
	void RegisterEntities();
};