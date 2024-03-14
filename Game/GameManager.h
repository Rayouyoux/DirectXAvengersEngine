#pragma once
#include <AvengersEngine.h>

namespace ave {
	class AvengersEngine;
	class GraphicsHandler;
	class EntityManager;
	class Entity;

	namespace Particles {
		struct ParticleBehaviour;
	}
}

class GameManager : public ave::Script {
private:
	static GameManager* Instance;

	ave::Entity* m_poMainCam;
	ave::Entity* m_poPlayer;

	ave::Entity* m_poRotCube;
	ave::Entity* m_poSkybox;

	ave::Entity* m_poParticleSystemEx;

	ave::Particles::ParticleBehaviour* m_poDefaultBehaviour;

public:
	GameManager();
	~GameManager();
	void Initialize(ave::AvengersEngine* engine);

	void Start() override;
	void Update(float dT) override;

	void InitResources();
	void InitEntities();
	void InitComponents();
	void RegisterEntities();

	static GameManager* Get() {
		return Instance;
	}

	ave::GraphicsHandler* m_poGraphics;
	ave::EntityManager* m_poManager;
};