#pragma once
#include <AvengersEngine.h>

class App : public ave::Entity {
private:
	static App* Instance;
	ave::GraphicsHandler* m_poGraphics;
	ave::EntityManager* m_poManager;

private:
	App();
	~App();

public:
	bool Create();
	void Initialize(ave::AvengersEngine& engine);

	void Start() override;
	void Update(float dT) override;

	void InitEntities();
	void InitComponents();
};