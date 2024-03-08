#pragma once
#include "UploadBuffer.h"

namespace ave {
	class Shader;
	struct PassConstants;
	class Camera : public Component
	{
	public:

		// Constructor
		Camera();

		// Deconstructor
		~Camera();

		// Frustrum Setter
		void SetLens(float fovY, float aspect, float zn, float zf);
		void ChangeAspectRatio(float fWidth, float fHeight);
		void Init();
		// SUR Methods
		void Start() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void Render() override;

		// Get Methods
		DirectX::XMMATRIX GetProjectionMatrix() const;

	private:

		// Matrices Update
		void UpdateProjectionMatrix();

	private:

		// Frustrum Properties
		float m_fAspect;
		float m_fFOV;
		float m_fNearZ;
		float m_fFarZ;
		float m_fNearWindowHeight;
		float m_fFarWindowHeight;

		UploadBuffer<PassConstants>* m_poBuffer;

		// View Matrix
		DirectX::XMFLOAT4X4 m_voProjectionMatrix;

		// Outdated View Flag
		bool m_bIsValidView;

	};
}
