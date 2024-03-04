#pragma once
#include "Component.h"
#include "ObjectPooler.h"

namespace ave {
	class Transform;
	class Shader;
	class Mesh;

	namespace Particles {
		enum ParticleOrientation {
			TowardsCamera,
		};

		enum ParticleRenderingType {
			Mesh,
			Texture,
			Flipbook
		};

		struct ParticleBehaviour {
			float MaxLifetime = 2;
			float Speed = 3;

			XMFLOAT3 Scale = { 1.f, 1.f, 1.f };
			bool ScaleOverTime;
			XMFLOAT3 EndScale = { 1, 0.2f, 0.2f };

			float Size = 1;
			bool SizeOverTime;
			float EndSize = 0.2f;

			ParticleRenderingType RenderingType = Mesh;
		};

		class Particle : public ObjectPooling::IPullable {
		public:
			Transform* m_poTransform;
			Shader* m_poShader;
			ave::Mesh* m_poMesh;
			float m_iLifetime;
			ParticleBehaviour* m_poBehaviour;

		public:
			Particle();
			~Particle();

			virtual void OnInstantiation() override;
			virtual void OnAcquire() override;
			virtual void OnRelease() override;

			void SetBehaviour(ParticleBehaviour* behaviour);
			void SetMesh(ave::Mesh* poMesh);
			void SetShader(Shader* poShader);

			virtual void Update(float deltaTime);
			virtual void Render();

			virtual void HandleMeshRendering();
		};

		class ParticleSystem : public Component {
		private:
			Particle* m_poParticleTemplate;

		public:

		private:

		public:
			void Start() override;
			void Update(float deltaTime) override;
			void LateUpdate(float deltaTime) override;
			void Render() override;
		};
	}
}