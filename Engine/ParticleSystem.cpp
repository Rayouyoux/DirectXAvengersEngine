#include <sstream>
#include "pch.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "GraphicsHandler.h"
#include "Shader.h"
#include "Mesh.h"
#include "Maths.h"
#include "Entity.h"
#include "ConstantsStruct.h"
#include "UploadBuffer.h"

namespace ave {
	using namespace Maths;

	namespace Particles {

#pragma region DefaultParticle

		Particle::Particle() {
			m_poTransform = new Transform();
			m_iLifetime = 0;
		}

		Particle::~Particle() {
			delete m_poTransform;
		}

		void Particle::Init() {
			XMFLOAT3 randomRotation;
			randomRotation.x = Random::Range(-360, 360);
			randomRotation.y = Random::Range(-360, 360);
			randomRotation.z = Random::Range(-360, 360);
			XMVECTOR vectorRot = DirectX::XMLoadFloat3(&randomRotation);
			m_poTransform->Rotate(&vectorRot);
			m_poTransform->UpdateMatrice();
		}

		//void Particle::OnInstantiation() {

		//}

		void Particle::SetBehaviour(ParticleBehaviour* behaviour) {
			m_poBehaviour = behaviour;

			m_fMaxLifetime = m_poBehaviour->MaxLifetime * Random::Range(-m_poBehaviour->LifetimeVariation, m_poBehaviour->LifetimeVariation);
			m_fSpeed = m_poBehaviour->Speed * (1 + Random::Range(-m_poBehaviour->SpeedVariation, m_poBehaviour->SpeedVariation));
			m_fEndSpeed = m_poBehaviour->EndSpeed * (1 + Random::Range(-m_poBehaviour->EndSpeedVariation, m_poBehaviour->EndSpeedVariation));
			m_fSize = m_poBehaviour->Size * (1 + Random::Range(-m_poBehaviour->SizeVariation, m_poBehaviour->SizeVariation));
			m_fEndSize = m_poBehaviour->EndSize * (1 + Random::Range(-m_poBehaviour->EndSizeVariation, m_poBehaviour->EndSizeVariation));
		}

		//void Particle::OnAcquire() {
		//	XMFLOAT3 randomRotation;
		//	randomRotation.x = Random::Range(-360, 360);
		//	randomRotation.y = Random::Range(-360, 360);
		//	randomRotation.z = Random::Range(-360, 360);
		//	XMVECTOR vectorRot = DirectX::XMLoadFloat3(&randomRotation);
		//	m_poTransform->Rotate(&vectorRot);
		//	m_poTransform->UpdateMatrice();

		//	//XMVECTOR scale = DirectX::XMLoadFloat3(&m_poBehaviour->Scale);
		//	//XMVECTOR vectorScale = scale * m_poBehaviour->Size;
		//	//m_poTransform->SetVectorScale(&vectorScale);
		//}

		//void Particle::OnRelease() {
		//	m_poTransform->Identity();
		//	m_iLifetime = 0;
		//}

		void Particle::Update(float deltaTime) {
			float alpha = m_iLifetime / m_fMaxLifetime;
			bool isMatrixDirty = true;

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
					size = Lerp<float>(m_fSize, m_fEndSize, alpha);
				}
				else {
					size = m_fSize;
				}
				XMVECTOR newScale = scale * size;
				m_poTransform->SetVectorScale(&newScale);
				isMatrixDirty = true;
			}

			float rotSpeed;
			if (m_poBehaviour->RotateOverTime) {
				rotSpeed = Lerp<float>(m_poBehaviour->RotSpeed,
					m_poBehaviour->EndRotSpeed, alpha);
			} else
				rotSpeed = m_poBehaviour->RotSpeed;
			if (rotSpeed != 0) {
				m_poTransform->RotateOnDir(rotSpeed * deltaTime);
				isMatrixDirty = true;
			}

			// Update position based on Speed along Dir
			float speed;
			if (m_poBehaviour->SpeedOverTime) {
				speed = Lerp<float>(m_fSpeed,
					m_fEndSpeed, alpha);
			} else
				speed = m_fSpeed;

			if (speed != 0) {
				XMVECTOR dir = m_poTransform->GetVectorDir();
				XMVECTOR offset = dir * speed * deltaTime;
				m_poTransform->Move(&offset);
				isMatrixDirty = true;
			}

			if (isMatrixDirty) {
				isMatrixDirty = false;
				m_poTransform->UpdateMatrice();
			}
		}

		void Particle::Render() {
		}

#pragma endregion

		ParticleSystem::ParticleSystem() {
			//m_poParticlePool = ObjectPooling::ObjectPool::Create();
			m_iCapacity = 0;
			m_fRateDebounce = 0.0f;
			m_lVerticesBuffer.resize(500*4);
			m_lIndicesBuffer.resize(500*6);
		}

		ParticleSystem::~ParticleSystem() {
			m_poVertexBufferGPU->Release();
			m_poIndexBufferGPU->Release();
			m_poVertexBufferUploader->Release();
			m_poIndexBufferUploader->Release();

			//m_poParticlePool->Release();
			m_lActiveParticles.clear();
			delete m_poBuffer;
		}

		void ParticleSystem::Initialize(GraphicsHandler* graphics) {
			m_fRateDebounce = 1 / m_poBehaviour->EmissionRate;
			//m_poParticlePool->Initialize<Particle>(iCapacity);
			m_poBuffer = new UploadBuffer<ObjectConstants>(
				GraphicsHandler::GetDevice(), 1, true);
		
			const UINT vbByteSize = (UINT)m_poBehaviour->MaxParticle * 4 * sizeof(VERTEX_UV);
			const UINT ibByteSize = (UINT)m_poBehaviour->MaxParticle * 6 * sizeof(std::uint16_t);

			m_iVertexByteStride = sizeof(VERTEX_UV);
			m_iVertexBufferByteSize = vbByteSize;
			m_eIndexFormat = DXGI_FORMAT_R16_UINT;
			m_iIndexBufferByteSize = ibByteSize;

			ID3D12Device* device = GraphicsHandler::GetDevice();
			ID3D12GraphicsCommandList* cmdList = GraphicsHandler::GetCommandList();
			
			graphics->ResetCommandList();

			m_poVertexBufferGPU = D3DUtils::CreateDefaultBuffer(device,
				cmdList, m_lVerticesBuffer.data(), vbByteSize, m_poVertexBufferUploader);

			m_poIndexBufferGPU = D3DUtils::CreateDefaultBuffer(device,
				cmdList, m_lIndicesBuffer.data(), ibByteSize, m_poIndexBufferUploader);
		
			graphics->CloseCommandList();

			ID3D12CommandList* cmdsLists[] = { cmdList };
			ID3D12CommandQueue* commandQueue = graphics->GetCommandQueue();
			commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
		
			graphics->FlushCommandQueue();
		}

		void ParticleSystem::SetBehaviour(ParticleBehaviour* poBehaviour) {
			m_poBehaviour = poBehaviour;
		}

		void ParticleSystem::SetShader(Shader* shader) {
			m_poShader = shader;
		}

		void ParticleSystem::SetTexture(Texture* texture) {
			m_poTexture = texture;
		}

		void ParticleSystem::Start(){
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
			m_poBuffer->CopyData(0, objConstants);
		}

		void ParticleSystem::Burst() {
			for (int i = 0; i < m_poBehaviour->BurstAmount; i++) {
				SpawnNewParticle();
			}
		}

		void ParticleSystem::SpawnNewParticle() {
			Particle* particle = new Particle();
			particle->Init();
			XMVECTOR pos = m_poEntity->m_poTransform->GetVectorPosition();
			particle->m_poTransform->SetVectorPosition(&pos);
			particle->SetBehaviour(m_poBehaviour);

			m_lActiveParticles.push_back(particle);
		}

		void ParticleSystem::Update(float deltaTime) {
			if (m_poBehaviour->EmissionType == ParticleEmissionType::EmissionRate) {
				m_fRateDebounce += deltaTime;
				if (m_fRateDebounce >= 1 / m_poBehaviour->EmissionRate) {
					m_fRateDebounce = 0;
					SpawnNewParticle();
				}
			}

			for (int i = m_lActiveParticles.size()-1; i >= 0; i--) {
				Particle* particle = m_lActiveParticles[i];
				particle->m_iLifetime += deltaTime;

				if (particle->m_iLifetime >= particle->m_fMaxLifetime) {
					m_lActiveParticles.erase(m_lActiveParticles.begin() + i);
					delete particle;
				}
				else {
					particle->Update(deltaTime);
				}
			}
		}

		void ParticleSystem::LateUpdate(float deltaTime)
		{}

		void ParticleSystem::Render() {
			m_lVerticesBuffer.clear();
			m_lIndicesBuffer.clear();

			Entity* camera = m_poShader->m_poCamera->GetEntity();
			XMVECTOR camPos = camera->m_poTransform->GetVectorPosition();

			for (int i = 0; i < m_lActiveParticles.size(); i++) {
				Particle* particle = m_lActiveParticles[i];

				// Rotate towards the camera
				XMVECTOR pos = particle->m_poTransform->GetVectorPosition();
				XMVECTOR cameraDir = camPos - pos;
				cameraDir = XMVector3Normalize(-cameraDir);

				Transform tmp = *particle->m_poTransform;
				tmp.LookTo(&cameraDir);

				XMFLOAT3 fPos, fSize;
				XMVECTOR vPos = tmp.GetVectorPosition();
				XMStoreFloat3(&fPos, vPos);

				XMVECTOR vSize = tmp.GetVectorScale() * 0.5f;
				XMStoreFloat3(&fSize, vSize);

				XMVECTOR up = tmp.GetVectorUp();
				XMVECTOR right = tmp.GetVectorRight();

				// Calculate vertices positions
				XMFLOAT3 fBottomLeft, fTopLeft, fTopRight, fBottomRight;
				XMVECTOR vBottomLeft = vPos - right * fSize.x - up * fSize.y;
				XMVECTOR vTopLeft = vPos - right * fSize.x + up * fSize.y;
				XMVECTOR vTopRight = vPos + right * fSize.x + up * fSize.y;
				XMVECTOR vBottomRight = vPos + right * fSize.x - up * fSize.y;
				XMStoreFloat3(&fBottomLeft, vBottomLeft);
				XMStoreFloat3(&fTopLeft, vTopLeft);
				XMStoreFloat3(&fTopRight, vTopRight);
				XMStoreFloat3(&fBottomRight, vBottomRight);

				//// Populate the vertices buffer ////
				// Bottom Left
				m_lVerticesBuffer.push_back(
					VERTEX_UV({ fBottomLeft, XMFLOAT4(0.f, 0.f, 0.f, 0.f) })
				);

				// Top Left
				m_lVerticesBuffer.push_back(
					VERTEX_UV({ fTopLeft, XMFLOAT4(1.f, 0.f, 0.f, 0.f) })
				);

				// Top Right
				m_lVerticesBuffer.push_back(
					VERTEX_UV({ fTopRight, XMFLOAT4(1.f, 1.f, 0.f, 0.f) })
				);

				// Bottom Right
				m_lVerticesBuffer.push_back(
					VERTEX_UV({ fBottomRight, XMFLOAT4(0.f, 1.f, 0.f, 0.f) })
				);
				
				//// Populate the indices buffer ////
				m_lIndicesBuffer.push_back(i*4);
				m_lIndicesBuffer.push_back(i*4+1);
				m_lIndicesBuffer.push_back(i*4+2);
				m_lIndicesBuffer.push_back(i*4);
				m_lIndicesBuffer.push_back(i*4+2);
				m_lIndicesBuffer.push_back(i*4+3);
			}

			void* data;
			m_poVertexBufferUploader->Map(0, nullptr, &data);
			memcpy(data, m_lVerticesBuffer.data(), m_lVerticesBuffer.size()*sizeof(VERTEX_UV));
			m_poVertexBufferUploader->Unmap(0, nullptr);

			m_poIndexBufferUploader->Map(0, nullptr, &data);
			memcpy(data, m_lIndicesBuffer.data(), m_lIndicesBuffer.size()*4);
			m_poIndexBufferUploader->Unmap(0, nullptr);

			UINT indexCount = (UINT)m_lIndicesBuffer.size();
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = m_poVertexBufferUploader->GetGPUVirtualAddress();
			vbv.StrideInBytes = m_iVertexByteStride;
			vbv.SizeInBytes = m_iVertexBufferByteSize;

			D3D12_INDEX_BUFFER_VIEW ibv;
			ibv.BufferLocation = m_poIndexBufferUploader->GetGPUVirtualAddress();
			ibv.Format = m_eIndexFormat;
			ibv.SizeInBytes = m_iIndexBufferByteSize;
			
			m_poShader->Draw(vbv, ibv, indexCount, m_poBuffer, m_poTexture);
		}
	}
}