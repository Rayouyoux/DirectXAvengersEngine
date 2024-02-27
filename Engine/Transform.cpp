#include "pch.h"
#include "Transform.h"
#include "Maths.h"

using namespace DirectX;

namespace ave {

	Transform::Transform(){
		Identity();
	}

	void Transform::Identity() {
		m_vScale = { 1.0f, 1.0f, 1.0f };
		m_mScale = maths::MatriceIdentity();

		m_vPosition = { 0.0f, 0.0f, 0.0f };
		m_mPosition = maths::MatriceIdentity();

		m_vUp = { 0.0f, 1.0f, 0.0f };
		m_vDir = { 0.0f, 0.0f, 1.0f };
		m_vRight = { 1.0f, 0.0f, 0.0f };
		m_qRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_mRotation = maths::MatriceIdentity();

		m_mTransformation = maths::MatriceIdentity();
	}

	void Transform::Move(XMFLOAT3* pvPosition) {
		Move(pvPosition->x, pvPosition->y, pvPosition->z);
	}

	void Transform::Move(float fX, float fY, float fZ){
		m_vPosition.x = fX;
		m_vPosition.y = fY;
		m_vPosition.z = fZ;
		XMStoreFloat4x4(&m_mRotation, XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z));
	}

	void Transform::Scale(XMFLOAT3* pvScale) {
		Scale(pvScale->x, pvScale->y, pvScale->z);
	}

	void Transform::Scale(float fX, float fY, float fZ) {
		m_vScale.x = fX;
		m_vScale.y = fY;
		m_vScale.z = fZ;
		XMStoreFloat4x4(&m_mScale, XMMatrixTranslation(m_vScale.x, m_vScale.y, m_vScale.z));
	}

	void Transform::Rotate(XMFLOAT3* pvRotate) {
		Rotate(pvRotate->x, pvRotate->y, pvRotate->z);
	}

	void Transform::Rotate(float fPitch, float fRoll, float fYaw) {

		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), fRoll);
		qQuaternion *= XMQuaternionRotationAxis(XMLoadFloat3(&m_vRight), fPitch);
		qQuaternion *= XMQuaternionRotationAxis(XMLoadFloat3(&m_vUp), fYaw);

		XMStoreFloat4(&m_qRotation, XMLoadFloat4(&m_qRotation) * qQuaternion);
		
		XMFLOAT4X4 mRotation;
		XMStoreFloat4x4(&mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));
		m_vRight.x = mRotation._11;
		m_vRight.y = mRotation._12;
		m_vRight.z = mRotation._13;
		m_vUp.x = mRotation._21;
		m_vUp.y = mRotation._22;
		m_vUp.z = mRotation._23;
		m_vDir.x = mRotation._31;
		m_vDir.y = mRotation._32;
		m_vDir.z = mRotation._33;

	}

	void Transform::UpdateMatrice() {

		XMMATRIX mScale = XMLoadFloat4x4(&m_mScale);
		XMMATRIX mPosition = XMLoadFloat4x4(&m_mPosition);
		XMMATRIX mRotation = XMLoadFloat4x4(&m_mRotation);
		XMMATRIX mWorld = mScale * mRotation * mPosition;

		XMStoreFloat4x4(&m_mTransformation, mWorld);
	}

	Transform::~Transform(){

	}
}
