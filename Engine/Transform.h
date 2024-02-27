#pragma once


#include <DirectXPackedVector.h>

using namespace DirectX;

namespace ave{

	class Transform {

	public:

		Transform();
		void Identity();
		void Rotate(float fPitch, float fRoll, float fYaw);
		void UpdateMatrice();
		~Transform();

		XMFLOAT4X4 m_mTransformation;

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

