#pragma once


#include <DirectXPackedVector.h>

using namespace DirectX;

namespace ave{

	class Transform {

	public:

		Transform();
		void Identity();
		void Move(FXMVECTOR* pvPosition);
		void Move(float fX, float fY, float fZ);
		void Scale(FXMVECTOR* pvScale);
		void Scale(float fX, float fY, float fZ);
		void Rotate(FXMVECTOR* pvRotate);
		void Rotate(float fPitch, float fRoll, float fYaw);
		void UpdateMatrice();

		XMMATRIX GetWorld() { return XMLoadFloat4x4(&m_mTransformation); };

		XMVECTOR GetVectorScale() { return XMLoadFloat3(&m_vScale); };
		XMMATRIX GetMatrixScale() { return XMLoadFloat4x4(&m_mScale); };

		XMVECTOR GetVectorPosition() { return XMLoadFloat3(&m_vPosition); };
		XMMATRIX GetMatrixPosition() { return XMLoadFloat4x4(&m_mPosition); };

		XMVECTOR GetVectorDir() { return XMLoadFloat3(&m_vDir); };
		XMVECTOR GetVectorUp() { return XMLoadFloat3(&m_vUp); };
		XMVECTOR GetVectorRight() { return XMLoadFloat3(&m_vRight); };
		XMVECTOR GetQuatRotation() { return XMLoadFloat4(&m_qRotation); };
		XMMATRIX GetMatrixRotation() { return XMLoadFloat4x4(&m_mRotation); };

		void SetVectorScale(FXMVECTOR* newScale);
		void SetMatrixScale(FXMMATRIX* newMatrixScale);

		void SetVectorPosition(FXMVECTOR* newPosition);
		void SetMatrixPosition(FXMMATRIX* newMatrixPosition);

		void RotateOnDir(float fRoll);
		void RotateOnUp(float fYaw);
		void RotateOnRight(float fPitch);
		void SetQuatRotation(FXMVECTOR* newRotation);
		void SetMatrixRotation(FXMMATRIX* newMatrixRotation);

		void LookTo(FXMVECTOR* poDirection);

		~Transform();

		XMFLOAT4X4 m_mTransformation;

		bool m_bHandleChange;

		bool m_bRotated = false;
		bool m_bMoved = false;
		bool m_bScaled = false;

	private:
		
		XMFLOAT3 m_vScale;
		XMFLOAT4X4 m_mScale;

		XMFLOAT3 m_vPosition;
		XMFLOAT4X4 m_mPosition;

		XMFLOAT3 m_vUp;
		XMFLOAT3 m_vDir;
		XMFLOAT3 m_vRight;
		XMFLOAT4 m_qRotation;
		XMFLOAT4X4 m_mRotation;
	};
}

