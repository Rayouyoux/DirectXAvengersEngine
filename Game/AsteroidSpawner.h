#pragma once
#include <AvengersEngine.h>

class AsteroidSpawner : public ave::Script {
protected:
	ave::EntityManager* m_poManager;
	float m_fSpawnRate;
	float m_fDebounce;
	ave::Particles::ParticleBehaviour* m_poBehaviour;

public:
	AsteroidSpawner();
	~AsteroidSpawner();

	void SetBehaviour(ave::Particles::ParticleBehaviour* behaviour) { m_poBehaviour = behaviour; }

	void Start() override;
	void Update(float dT) override;

	void SpawnNewAsteroid();
};