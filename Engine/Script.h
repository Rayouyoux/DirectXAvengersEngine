#pragma once
#include "Component.h"

namespace ave {
	class Script : public Component {
	public:
		virtual void Start() override {};
		virtual void Update(float dT) override {};
		virtual void LateUpdate(float dT) override {};
		virtual void Render() override {};
		virtual void Render2D() override {};

		virtual void OnCollisionEnter(Entity* collider) override {};
		virtual void OnCollisionStay(Entity* collider) override {};
		virtual void OnCollisionExit(Entity* collider) override {};
	};
}