#include "pch.h"
#include "Transform.h"
#include "Maths.h"

using namespace DirectX;

namespace ave {

	Transform::Transform(){
		m_bHandleChange = false;
		Identity();
	}

	void Transform::Identity() {
		m_vScale = { 1.0f, 1.0f, 1.0f };
		m_mScale = Maths::MatriceIdentity();

		m_vPosition = { 0.0f, 0.0f, 0.0f };
		m_mPosition = Maths::MatriceIdentity();

		m_vUp = { 0.0f, 1.0f, 0.0f };
		m_vDir = { 0.0f, 0.0f, 1.0f };
		m_vRight = { 1.0f, 0.0f, 0.0f };
		m_qRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_mRotation = Maths::MatriceIdentity();

		m_mTransformation = Maths::MatriceIdentity();
	}

	void Transform::Move(FXMVECTOR* pvPosition) {
		XMFLOAT3 pvPos;
		XMStoreFloat3(&pvPos, *pvPosition);
		Move(pvPos.x, pvPos.y, pvPos.z);
		m_bHandleChange = true;
	}

	void Transform::Move(float fX, float fY, float fZ) {
		m_vPosition.x += fX;
		m_vPosition.y += fY;
		m_vPosition.z += fZ;
		XMStoreFloat4x4(&m_mPosition, XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z));
	}

	void Transform::Scale(FXMVECTOR* pvScale) {
		XMFLOAT3 pvScl;
		XMStoreFloat3(&pvScl, *pvScale);
		Scale(pvScl.x, pvScl.y, pvScl.z);
		m_bHandleChange = true;
	}

	void Transform::Scale(float fX, float fY, float fZ) {
		m_vScale.x += fX;
		m_vScale.y += fY;
		m_vScale.z += fZ;
		XMStoreFloat4x4(&m_mScale, XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z));
	}

	void Transform::Rotate(FXMVECTOR* pvRotate) {
		XMFLOAT3 pvRot;
		XMStoreFloat3(&pvRot, *pvRotate);
		Rotate(pvRot.x, pvRot.y, pvRot.z);
		m_bHandleChange = true;
	}

	void Transform::Rotate(float fPitch, float fRoll, float fYaw) {

		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), fRoll);
		qQuaternion = XMQuaternionMultiply(qQuaternion, XMQuaternionRotationAxis(XMLoadFloat3(&m_vRight), fPitch));
		qQuaternion = XMQuaternionMultiply(qQuaternion, XMQuaternionRotationAxis(XMLoadFloat3(&m_vUp), fYaw));

		XMStoreFloat4(&m_qRotation, XMQuaternionMultiply(XMLoadFloat4(&m_qRotation), qQuaternion));

		XMStoreFloat4x4(&m_mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));

		m_vRight.x = m_mRotation._11;
		m_vRight.y = m_mRotation._12;
		m_vRight.z = m_mRotation._13;
		m_vUp.x = m_mRotation._21;
		m_vUp.y = m_mRotation._22;
		m_vUp.z = m_mRotation._23;
		m_vDir.x = m_mRotation._31;
		m_vDir.y = m_mRotation._32;
		m_vDir.z = m_mRotation._33;
	}

	void Transform::UpdateMatrice() {

		XMMATRIX mScale = XMLoadFloat4x4(&m_mScale);
		XMMATRIX mPosition = XMLoadFloat4x4(&m_mPosition);
		XMMATRIX mRotation = XMLoadFloat4x4(&m_mRotation);
		XMMATRIX mWorld = XMMatrixMultiply(XMMatrixMultiply(mScale, mRotation), mPosition);
		XMStoreFloat4x4(&m_mTransformation, mWorld);
		m_bHandleChange = false;
	}

	void Transform::SetVectorScale(FXMVECTOR* newScale) { 
		XMStoreFloat3(&m_vScale, *newScale); 
		XMStoreFloat4x4(&m_mScale, XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z));
	};

	void Transform::SetMatrixScale(FXMMATRIX* newMatrixScale) { 
		XMStoreFloat4x4(&m_mScale, *newMatrixScale);
		m_vScale.x = m_mScale._11;
		m_vScale.y = m_mScale._22;
		m_vScale.z = m_mScale._33;
		m_bHandleChange = true;
	};

	void Transform::SetVectorPosition(FXMVECTOR* newPosition) { 
		XMStoreFloat3(&m_vPosition, *newPosition);
		XMStoreFloat4x4(&m_mPosition, XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z));
		m_bHandleChange = true;
	};

	void Transform::SetMatrixPosition(FXMMATRIX* newMatrixPosition) { 
		XMStoreFloat4x4(&m_mPosition, *newMatrixPosition); 
		m_vPosition.x = m_mPosition._41;
		m_vPosition.y = m_mPosition._42;
		m_vPosition.z = m_mPosition._43;
		m_bHandleChange = true;
	};

	void Transform::RotateOnDir(float fRoll) {
		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vDir), fRoll);

		XMStoreFloat4(&m_qRotation, XMQuaternionMultiply(XMLoadFloat4(&m_qRotation), qQuaternion));

		XMStoreFloat4x4(&m_mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));

		m_vDir.x = m_mRotation._31;
		m_vDir.y = m_mRotation._32;
		m_vDir.z = m_mRotation._33;
		m_bHandleChange = true;
	};

	void Transform::RotateOnUp(float fYaw) {
		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vUp), fYaw);

		XMStoreFloat4(&m_qRotation, XMQuaternionMultiply(XMLoadFloat4(&m_qRotation), qQuaternion));

		XMStoreFloat4x4(&m_mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));

		m_vUp.x = m_mRotation._21;
		m_vUp.y = m_mRotation._22;
		m_vUp.z = m_mRotation._23;
		m_bHandleChange = true;
	};

	void Transform::RotateOnRight(float fPitch) {
		XMVECTOR qQuaternion;
		qQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&m_vRight), fPitch);

		XMStoreFloat4(&m_qRotation, XMQuaternionMultiply(XMLoadFloat4(&m_qRotation), qQuaternion));

		XMStoreFloat4x4(&m_mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));

		m_vRight.x = m_mRotation._11;
		m_vRight.y = m_mRotation._12;
		m_vRight.z = m_mRotation._13; 
		m_bHandleChange = true;
	};

	void Transform::SetQuatRotation(FXMVECTOR* newRotation) { 
		XMStoreFloat4(&m_qRotation, *newRotation);

		XMStoreFloat4x4(&m_mRotation, XMMatrixRotationQuaternion(XMLoadFloat4(&m_qRotation)));

		m_vRight.x = m_mRotation._11;
		m_vRight.y = m_mRotation._12;
		m_vRight.z = m_mRotation._13;
		m_vUp.x = m_mRotation._21;
		m_vUp.y = m_mRotation._22;
		m_vUp.z = m_mRotation._23;
		m_vDir.x = m_mRotation._31;
		m_vDir.y = m_mRotation._32;
		m_vDir.z = m_mRotation._33;
		m_bHandleChange = true;
	};

	void Transform::SetMatrixRotation(FXMMATRIX* newMatrixRotation) { 
		XMStoreFloat4x4(&m_mRotation, *newMatrixRotation);

		XMStoreFloat4(&m_qRotation, XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_mRotation)));

		m_vRight.x = m_mRotation._11;
		m_vRight.y = m_mRotation._12;
		m_vRight.z = m_mRotation._13;
		m_vUp.x = m_mRotation._21;
		m_vUp.y = m_mRotation._22;
		m_vUp.z = m_mRotation._23;
		m_vDir.x = m_mRotation._31;
		m_vDir.y = m_mRotation._32;
		m_vDir.z = m_mRotation._33;
		m_bHandleChange = true;
	};

	void Transform::LookTo(FXMVECTOR* poDirection) {
		XMMATRIX mRot = XMMatrixLookToLH(XMVectorZero(), *poDirection, XMLoadFloat3(&m_vUp));
		mRot = XMMatrixInverse(nullptr, mRot);
		XMStoreFloat4x4(&m_mRotation, mRot);

		XMStoreFloat4(&m_qRotation, XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_mRotation)));

		m_vRight.x = m_mRotation._11;
		m_vRight.y = m_mRotation._12;
		m_vRight.z = m_mRotation._13;
		m_vUp.x = m_mRotation._21;
		m_vUp.y = m_mRotation._22;
		m_vUp.z = m_mRotation._23;
		m_vDir.x = m_mRotation._31;
		m_vDir.y = m_mRotation._32;
		m_vDir.z = m_mRotation._33;

		m_bHandleChange = true;
	}

	Transform::~Transform(){

	}
}
