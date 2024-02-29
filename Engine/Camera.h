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
		void				Start() override;
		void				Update(__int64 deltaTime) override;
		void				LateUpdate(__int64 deltaTime) override;
		void				Render() override;

		// Get Methods
		DirectX::XMVECTOR	GetPosition() const;
		DirectX::XMFLOAT3	GetPosition3f() const;

		DirectX::XMMATRIX	GetViewMatrix() const;
		DirectX::XMMATRIX	GetProjectionMatrix() const;

		// Set Methods
		void				SetPosition(float x, float y, float z);
		void				SetPosition(const DirectX::XMFLOAT3& position);

		// Set Orientation based on a Target or a Vector
		void				LookAt(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
		void				LookAt(DirectX::FXMVECTOR position, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up);

		// Move Camera
		void				Strafe(float distance);
		void				Walk(float distance);

		// Rotate Camera
		void				Pitch(float angle);
		void				RotateY(float angle);

	private:

		// Matrices Update
		void				UpdateViewMatrix();
		void				UpdateProjectionMatrix();
			
	private:

		// Camera Position & Orientation
		DirectX::XMFLOAT3	m_voPosition;
		DirectX::XMFLOAT3	m_voRight;
		DirectX::XMFLOAT3	m_voUp;
		DirectX::XMFLOAT3	m_voLook;

		// Frustrum Properties
		float				m_fAspect;
		float				m_fFOV;
		float				m_fNearZ;
		float				m_fFarZ;
		float				m_fNearWindowHeight;
		float				m_fFarWindowHeight;

		// View Matrix
		DirectX::XMFLOAT4X4	m_voViewMatrix;
		DirectX::XMFLOAT4X4	m_voProjectionMatrix;

		// Outdated View Flag
		bool				m_bIsValidView;

	};
}

