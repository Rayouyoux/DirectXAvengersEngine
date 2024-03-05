#include "pch.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "GraphicsHandler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Maths.h"
#include "Entity.h"
#include "ConstantsStruct.h"

namespace ave {
	using namespace Maths;

	namespace Particles {

#pragma region DefaultParticle

		Particle::Particle() {
			m_poTransform = new Transform();
		}

		Particle::~Particle()
		{}

		void Particle::OnInstantiation() {

		}

		void Particle::SetBehaviour(ParticleBehaviour* behaviour) {
			m_poBehaviour = behaviour;
		}

		void Particle::SetMesh(ave::Mesh* poMesh) {
			m_poMesh = poMesh;
		}

		void Particle::SetShader(Shader* poShader) {
			m_poShader = poShader;
		}

		void Particle::OnAcquire() {
			XMFLOAT3 randomRotation;
			randomRotation.x = Random::Range(-360, 360);
			randomRotation.y = Random::Range(-360, 360);
			randomRotation.z = Random::Range(-360, 360);
			XMVECTOR vectorRot = DirectX::XMLoadFloat3(&randomRotation);
			m_poTransform->Rotate(&vectorRot);
			m_poTransform->UpdateMatrice();

			//XMVECTOR scale = DirectX::XMLoadFloat3(&m_poBehaviour->Scale);
			//XMVECTOR vectorScale = scale * m_poBehaviour->Size;
			//m_poTransform->SetVectorScale(&vectorScale);
		}

		void Particle::OnRelease() {
			m_poTransform->Identity();
			m_iLifetime = 0;
		}

		void Particle::Update(float deltaTime) {
			float alpha = m_iLifetime / m_poBehaviour->MaxLifetime;

			// Update size along lifetime
			if (m_poBehaviour->ScaleOverTime || m_poBehaviour->SizeOverTime) {
				XMVECTOR scale;
				if (m_poBehaviour->ScaleOverTime) {
					XMVECTOR startScale = DirectX::XMLoadFloat3(&m_poBehaviour->Scale);
					XMVECTOR endScale = DirectX::XMLoadFloat3(&m_poBehaviour->EndScale);
					scale = Lerp<XMVECTOR>(startScale, endScale, alpha);
				}
				else {
					scale = DirectX::XMLoadFloat3(&m_poBehaviour->Scale);
				}

				float size;
				if (m_poBehaviour->SizeOverTime) {
					size = Lerp<float>(m_poBehaviour->Size, m_poBehaviour->EndSize, alpha);
				}
				else {
					size = m_poBehaviour->Size;
				}
				XMVECTOR newScale = scale * size;
				m_poTransform->SetVectorScale(&newScale);
				m_poTransform->UpdateMatrice();
			}

			float rotSpeed;
			if (m_poBehaviour->RotateOverTime) {
				rotSpeed = Lerp<float>(m_poBehaviour->RotSpeed,
					m_poBehaviour->EndRotSpeed, alpha);
			} else
				rotSpeed = m_poBehaviour->RotSpeed;
			if (rotSpeed != 0) {
				m_poTransform->RotateOnDir(rotSpeed * deltaTime);
			}

			// Update position based on Speed along Dir
			float speed;
			if (m_poBehaviour->SpeedOverTime) {
				speed = Lerp<float>(m_poBehaviour->Speed,
					m_poBehaviour->EndSpeed, alpha);
			} else
				speed = m_poBehaviour->Speed;

			if (m_poBehaviour->Speed != 0) {
				XMVECTOR dir = m_poTransform->GetVectorDir();
				XMVECTOR offset = dir * speed * deltaTime;
				m_poTransform->Move(&offset);
				m_poTransform->UpdateMatrice();
			}

			// Render Update
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poTransform->GetWorld()));
			m_poShader->UpdateObject(objConstants);
		}

		void Particle::Render() {
			switch (m_poBehaviour->RenderingType) {
			case Mesh:
				HandleMeshRendering();
				break;
			case Texture:
				break;
			case Flipbook:
				break;
			}
		}

		void Particle::HandleMeshRendering() {
			ID3D12GraphicsCommandList* poList = GraphicsHandler::GetCommandList();

			poList->SetGraphicsRootSignature(m_poShader->GetRootSignature());

			////Pass
			poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootPass(), m_poShader->GetPass()->Resource()->GetGPUVirtualAddress());

			////Pipeline
			poList->SetPipelineState(m_poShader->GetPso());

			////Topology
			poList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			auto oVertexBufferView = m_poMesh->VertexBufferView();
			auto oIndexBufferView = m_poMesh->IndexBufferView();
			poList->IASetVertexBuffers(0, 1, &oVertexBufferView);
			poList->IASetIndexBuffer(&oIndexBufferView);

			poList->SetGraphicsRootConstantBufferView(m_poShader->GetRootObject(), m_poShader->GetVirtualAdress());

			poList->DrawIndexedInstanced(m_poMesh->GetIndexCount(), 1, 0, 0, 0);
		}

#pragma endregion

		ParticleSystem::ParticleSystem() {
			m_poParticlePool = ObjectPooling::ObjectPool::Create();
			m_iCapacity = 0;
			m_iEmissionRate = 0;
			m_fRateDebounce = 0.0f;
		}

		ParticleSystem::~ParticleSystem() {
			m_poParticlePool->Release();
		}

		void ParticleSystem::Initialize(float iRate, int iCapacity) {
			m_iCapacity = iCapacity;
			m_iEmissionRate = iRate;
			m_fRateDebounce = 1 / m_iEmissionRate;
			m_poParticlePool->Initialize<Particle>(iCapacity);
		}

		void ParticleSystem::SetBehaviour(ParticleBehaviour* poBehaviour) {
			m_poBehaviour = poBehaviour;
		}

		void ParticleSystem::SetMesh(ave::Mesh* poMesh) {
			m_poMesh = poMesh;
		}

		void ParticleSystem::SetShader(Shader* shader) {
			m_poShader = shader;
		}

		void ParticleSystem::Start()
		{}

		void ParticleSystem::Update(float deltaTime) {
			m_fRateDebounce += deltaTime;
			if (m_fRateDebounce >= 1 / m_iEmissionRate) {
				m_fRateDebounce -= 1 / m_iEmissionRate;
				Particle* particle = m_poParticlePool->AcquireObject<Particle>();
				XMVECTOR pos = m_poEntity->m_poTransform->GetVectorPosition();
				particle->m_poTransform->SetVectorPosition(&pos);
				particle->SetBehaviour(m_poBehaviour);
				particle->SetMesh(m_poMesh);
				particle->SetShader(m_poShader);

				m_lActiveParticles.push_back(particle);
			}

			for (int i = m_lActiveParticles.size()-1; i >= 0; i--) {
				Particle* particle = m_lActiveParticles[i];
				particle->m_iLifetime += deltaTime;

				if (particle->m_iLifetime >= particle->m_poBehaviour->MaxLifetime) {
					m_lActiveParticles.erase(m_lActiveParticles.begin() + i);
					m_poParticlePool->ReleaseObject(particle);
				}
				else {
					particle->Update(deltaTime);
				}
			}
		}

		void ParticleSystem::LateUpdate(float deltaTime)
		{}

		void ParticleSystem::Render() {
			for (int i = 0; i < m_lActiveParticles.size(); i++) {
				Particle* particle = m_lActiveParticles[i];
				particle->Render();
			}
		}
	}
}