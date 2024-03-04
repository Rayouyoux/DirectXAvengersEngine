#include "pch.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "GraphicsHandler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Maths.h"

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
			randomRotation.x = Random::Range<float>(-360, 360);
			randomRotation.y = Random::Range<float>(-360, 360);
			randomRotation.z = Random::Range<float>(-360, 360);
			m_poTransform->Rotate(&randomRotation);

			XMVECTOR scale = XMLoadFloat3(&m_poBehaviour->Scale);
			m_poTransform.SetVectorScale(scale * m_poBehaviour->Size);
		}

		void Particle::OnRelease() {
			m_poTransform->Identity();
		}

		void Particle::Update(float deltaTime) {
			float alpha = m_iLifetime / m_poBehaviour->MaxLifetime;

			// Update size along lifetime
			if (m_poBehaviour->ScaleOverTime || m_poBehaviour->SizeOverTime) {
				XMVECTOR scale;
				if (m_poBehaviour->ScaleOverTime) {
					XMVECTOR startScale = XMLoadFloat3(&m_poBehaviour->Scale);
					XMVECTOR endScale = XMLoadFloat3(&m_poBehaviour->EndScale);
					scale = Lerp<XMVECTOR>(startScale, endScale, alpha);
				}
				else {
					scale = XMLoadFloat3(&m_poBehaviour->Scale);
				}

				float size;
				if (m_poBehaviour->SizeOverTime) {
					size = Lerp<float>(m_poBehaviour->Size, m_poBehaviour->EndSize, alpha);
				}
				else {
					size = m_poBehaviour->Size;
				}
				m_poTransform->Set
			}

			// Update position based on Speed along Dir
			if (m_poBehaviour->Speed != 0) {
				XMVECTOR dir = m_poTransform.GetDir();
				m_poTransform->Move(dir * m_poBehaviour->Speed * deltaTime);
			}
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

		void ParticleSystem::Start() {

		}

		void ParticleSystem::Update(float deltaTime) {
			// Add lifetime and delete particles
		}

		void ParticleSystem::LateUpdate(float deltaTime) {

		}

		void ParticleSystem::Render() {

		}
	}
}