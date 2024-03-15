#pragma once
#include "Component.h"
#include <vector>
#include <map>

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

		static std::vector<Collider*> m_vCollidables;
		std::map<Collider*, CollideState> m_mColliding;
	};

}