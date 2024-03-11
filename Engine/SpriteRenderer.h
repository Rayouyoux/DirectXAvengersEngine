#pragma once
#include "Component.h"

namespace ave {
	class SpriteRenderer : Component {
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Render() override;
	};
}