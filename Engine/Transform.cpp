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

	void Transform::Rotate(float fPitch, float fRoll, float fYaw) {

		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), fRoll);
		qQuaternion *= XMQuaternionRotationAxis(XMLoadFloat3(&m_vRight), fPitch);
		qQuaternion *= XMQuaternionRotationAxis(XMLoadFloat3(&m_vUp), fYaw);

		XMStoreFloat4(&m_qRotation, qQuaternion);
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
