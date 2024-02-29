#include "pch.h"
#include "Camera.h"

namespace ave {

	Camera::Camera() :
		m_fAspect(0.0f),
		m_fFOV(0.0f),
		m_fNearZ(0.0f),
		m_fFarZ(0.0f),
		m_fNearWindowHeight(0.0f),
		m_fFarWindowHeight(0.0f)
	{}

	Camera::~Camera()
	{}

	void Camera::SetLens(float fov, float aspect, float zn, float zf)
	{
		m_fFOV = fov;
		m_fAspect = aspect;
		m_fNearZ = zn;
		m_fFarZ = zf;

		m_fNearWindowHeight = 2.0f * m_fNearZ * tanf(0.5f * m_fFOV);
		m_fFarWindowHeight = 2.0f * m_fFarZ * tanf(0.5f * m_fFOV);

		XMMATRIX P = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspect, m_fNearZ, m_fFarZ);
		XMStoreFloat4x4(&m_voProjectionMatrix, P);
	}

	void Camera::Start()
	{
		SetLens(0.25f * maths::PI, 1.0f, 1.0f, 1000.0f);
	}

	void Camera::Update(__int64 deltaTime)
	{

	}
	
	void Camera::LateUpdate(__int64 deltaTime)
	{

	}

	void Camera::Render()
	{

	}

	DirectX::XMVECTOR Camera::GetPosition() const
	{
		return XMLoadFloat3(&m_voPosition);
	}

	DirectX::XMFLOAT3 Camera::GetPosition3f() const
	{
		return m_voPosition;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_voPosition = XMFLOAT3(x, y, z);
		m_bIsValidView = false;
	}

	void Camera::SetPosition(const DirectX::XMFLOAT3& position)
	{
		m_voPosition = position;
		m_bIsValidView = false;
	}

	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return DirectX::XMMATRIX();
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return DirectX::XMMATRIX();
	}

	void Camera::UpdateViewMatrix()
	{
	}

	void Camera::UpdateProjectionMatrix()
	{
	}
}
