#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Component.h"
#include "ObjectPooler.h"
#include "UploadBuffer.h"
#include "ConstantsStruct.h"

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
			float MaxLifetime = 0.4;

			float Speed = -15;
			bool SpeedOverTime = false;
			float EndSpeed = 10;

			float RotSpeed = 10;
			bool RotateOverTime = false;
			float EndRotSpeed = 80;

			const DirectX::XMFLOAT3 Scale = { 1.f, 1.f, 1.f };
			bool ScaleOverTime = false;
			const DirectX::XMFLOAT3 EndScale = { 0, 0, 0 };

			float Size = .8f;
			bool SizeOverTime = true;
			float EndSize = 0;

			ParticleRenderingType RenderingType = Mesh;
		};

		class Particle : public ObjectPooling::IPullable {
		public:
			Transform* m_poTransform;
			Shader* m_poShader;
			UploadBuffer<ObjectConstants>* m_poBuffer;
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

			void SpawnInsideSphere(float fRadius);

			virtual void Update(float deltaTime);
			virtual void Render();

			virtual void HandleMeshRendering();
		};

		class ParticleSystem : public Component {
		private:
			ObjectPooling::ObjectPool* m_poParticlePool;
			int m_iCapacity;
			float m_iEmissionRate;

			ParticleBehaviour* m_poBehaviour;
			ave::Mesh* m_poMesh;
			Shader* m_poShader;

			double m_fRateDebounce;

			std::vector<Particle*> m_lActiveParticles;

		public:
			ParticleSystem();
			~ParticleSystem();

			void Initialize(float iRate, int iCapacity);

			void SetBehaviour(ParticleBehaviour* behaviour);
			void SetMesh(ave::Mesh* mesh);
			void SetShader(Shader* shader);

			void Start() override;
			void Update(float deltaTime) override;
			void LateUpdate(float deltaTime) override;
			void Render() override;
		};
	}
}