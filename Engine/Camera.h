#pragma once
#include "UploadBuffer.h"
#include "Component.h"

namespace ave {
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
		
		void Start() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void Render() override;

		// Get Methods
		DirectX::XMMATRIX GetProjectionMatrix() const;

		UploadBuffer<PassConstants>* m_poBuffer;

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

		// View Matrix
		DirectX::XMFLOAT4X4 m_voProjectionMatrix;

		// Outdated View Flag
		bool m_bIsValidView;
	};
}
