#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Component.h"
#include "ObjectPooler.h"
#include "UploadBuffer.h"
#include "ConstantsStruct.h"
#include "Vertex.h"
#include <dxgi.h>

class ID3D12Resource;
class ID3D10Blob;

namespace ave {
	class Transform;
	class Shader;
	class Texture;
	class GraphicsHandler;

	namespace Particles {
		enum ParticleOrientation {
			TowardsCamera,
		};

		struct ParticleBehaviour {
			int MaxParticle = 500;
			float MaxLifetime = 0.8;

			float Speed = 6;
			bool SpeedOverTime = false;
			float EndSpeed = 2;

			float RotSpeed = 20;
			bool RotateOverTime = false;
			float EndRotSpeed = 100;

			const DirectX::XMFLOAT3 Scale = { 0.2f, 0.2f, 1.f };
			bool ScaleOverTime = false;
			const DirectX::XMFLOAT3 EndScale = { 1, 1, 1 };

			float Size = 0.5f;
			bool SizeOverTime = true;
			float EndSize = 0;
		};

		class Particle {
		public:
			Transform* m_poTransform;
			float m_iLifetime;
			ParticleBehaviour* m_poBehaviour;

		public:
			Particle();
			~Particle();

			void Init();

			void SetBehaviour(ParticleBehaviour* behaviour);

			virtual void Update(float deltaTime);
			virtual void Render();
		};

		class ParticleSystem : public Component {
		private:
			ObjectPooling::ObjectPool* m_poParticlePool;
			int m_iCapacity;
			float m_iEmissionRate;

			ParticleBehaviour* m_poBehaviour;
			Shader* m_poShader;
			Texture* m_poTexture;

			float m_fRateDebounce;
			UploadBuffer<ObjectConstants>* m_poBuffer;

			std::vector<Particle*> m_lActiveParticles;
			std::vector<VERTEX_UV> m_lVerticesBuffer;
			std::vector<uint16_t> m_lIndicesBuffer;

			ID3D12Resource* m_poVertexBufferGPU;
			ID3D12Resource* m_poIndexBufferGPU;

			ID3D12Resource* m_poVertexBufferUploader;
			ID3D12Resource* m_poIndexBufferUploader;

			UINT m_iVertexByteStride;
			UINT m_iVertexBufferByteSize;

			DXGI_FORMAT m_eIndexFormat;
			UINT m_iIndexBufferByteSize;

		public:
			ParticleSystem();
			~ParticleSystem();

			void Initialize(GraphicsHandler* graphics, float iRate, int iCapacity);

			void SetBehaviour(ParticleBehaviour* behaviour);
			void SetShader(Shader* shader);
			void SetTexture(Texture* texture);

			void Start() override;
			void Update(float deltaTime) override;
			void LateUpdate(float deltaTime) override;
			void Render() override;
		};
	}
}