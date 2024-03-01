#pragma once

#include <DirectXMath.h>

namespace ave {

	class Camera : public Component
	{
	public:

		// Constructor
		Camera();

		// Deconstructor
		~Camera();

		// Frustrum Setter
		void SetLens(float fovY, float aspect, float zn, float zf);

		// SUR Methods
		void Start() override;
		void Update(__int64 deltaTime) override;
		void LateUpdate(__int64 deltaTime) override;
		void Render() override;

		// Get Methods
		DirectX::XMFLOAT4X4 GetProjectionMatrix() const;

	private:

		// Matrice Update
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
