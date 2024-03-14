#include "pch.h"
#include "Component.h"
#include "Camera.h"
#include "Maths.h"
#include "Entity.h"
#include "Transform.h"
#include "GraphicsHandler.h"
#include "Shader.h"
#include "ConstantsStruct.h"

namespace ave {

	Camera::Camera() 
		: Component()
	{
		m_fAspect = 0.0f;
		m_fFOV = 0.0f;
		m_fNearZ = 0.0f;
		m_fFarZ = 0.0f;
		m_fNearWindowHeight = 0.0f;
		m_fFarWindowHeight = 0.0f;
		m_bIsValidView = true;
		m_voPerspectiveProj = Maths::MatriceIdentity();
	}

	Camera::~Camera(){
		delete m_poBuffer;
	}

	void Camera::SetLens(float fov, float aspect, float zn, float zf)
	{
		m_fFOV = XMConvertToRadians(fov);
		m_fAspect = aspect;
		m_fNearZ = zn;
		m_fFarZ = zf;

		m_fNearWindowWidth = 2.0f * m_fNearZ * tanf(0.5f * m_fFOV) * m_fAspect;
		m_fNearWindowHeight = 2.0f * m_fNearZ * tanf(0.5f * m_fFOV);
		m_fFarWindowHeight = 2.0f * m_fFarZ * tanf(0.5f * m_fFOV);

		XMMATRIX P = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspect, m_fNearZ, m_fFarZ);
		XMStoreFloat4x4(&m_voPerspectiveProj, P);

		XMMATRIX O = XMMatrixOrthographicLH( 800.0f, 600.0f, m_fNearZ, m_fFarZ);
		XMStoreFloat4x4(&m_voOrthographicProj, O);
	}

	void Camera::Start()
	{
		SetLens(70, 800.f/600.f, 1.0f, 1000.0f);
		m_poBuffer = new UploadBuffer<PassConstants>(GraphicsHandler::GetDevice(), 1, true);
		m_poBufferSprite = new UploadBuffer<PassConstants>(GraphicsHandler::GetDevice(), 1, true);
	}

	void Camera::ChangeAspectRatio(float fWidth, float fHeight) {
		SetLens(70, fWidth / fHeight, 1.0f, 1000.0f);
	}

	void Camera::Update(float deltaTime) {
	
	}
	
	void Camera::LateUpdate(float deltaTime){

	}

	void Camera::Render() {
		PassConstants opassConstants;
		XMStoreFloat4x4(&opassConstants.View, XMMatrixTranspose(m_poEntity->m_poTransform->GetWorld()));
		XMStoreFloat4x4(&opassConstants.Proj, XMMatrixTranspose(XMLoadFloat4x4(&m_voPerspectiveProj)));
		m_poBuffer->CopyData(0, opassConstants);
	}

	void Camera::Render2D() {
		PassConstants opassConstants;
		XMFLOAT4X4 voViewIdentity = Maths::MatriceIdentity();
		XMStoreFloat4x4(&opassConstants.View, XMMatrixTranspose(XMLoadFloat4x4(&voViewIdentity)));
		XMStoreFloat4x4(&opassConstants.Proj, XMMatrixTranspose(XMLoadFloat4x4(&m_voOrthographicProj)));
		m_poBufferSprite->CopyData(0, opassConstants);
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return DirectX::XMLoadFloat4x4(&m_voPerspectiveProj);
	}

	void Camera::UpdateProjectionMatrix()
	{}
}