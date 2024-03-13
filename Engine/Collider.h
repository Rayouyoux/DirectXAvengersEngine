#pragma once
#include "Component.h"

namespace ave {

	enum class CollideState {
		Away,
		Enter,
		Stay,
		Exit
	};

	class Collider : public Component
	{
	public:

		Collider();
		~Collider();

		void Start() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void Render() override;

		void OnCollisionEnter(Entity* collider) override;
		void OnCollisionStay(Entity* collider) override;
		void OnCollisionExit(Entity* collider) override;

		static std::vector<Collider*> m_vCollidables;
		std::map<Collider*, CollideState> m_mColliding;
	};

}

