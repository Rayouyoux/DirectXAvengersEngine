#include "pch.h"
#include "Component.h"
#include "Camera.h"
#include "Maths.h"
#include "Entity.h"
#include "Transform.h"

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
		m_voProjectionMatrix = maths::MatriceIdentity();
	}

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

	void Camera::Update(float deltaTime)
	{
		UpdateProjectionMatrix();
	}
	
	void Camera::LateUpdate(float deltaTime)
	{}

	void Camera::Render()
	{}

	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return DirectX::XMLoadFloat4x4(&m_voProjectionMatrix);
	}

	void Camera::UpdateProjectionMatrix()
	{}
}

// &m_poEntity->m_poTransform->m_bHandleChange

/*
	InputManager::InputManager()
		: m_mouseCaptured(false), m_lastMouseX(0), m_lastMouseY(0)
	{
		ZeroMemory(m_keyState, sizeof(m_keyState));

		m_cameraPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_cameraRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

      InputManager::~InputManager()
    {}

    void InputManager::Initialize(HWND hwnd)
    {
        m_mouseCaptured = false;

        m_lastMouseX = 0;
        m_lastMouseY = 0;

        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01;
        rid.usUsage = 0x02;
        rid.dwFlags = 0;
        rid.hwndTarget = hwnd;
        RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
    }

    void InputManager::Update()
    {
        if (m_mouseCaptured)
        {
            int dx = m_lastMouseX - m_lastMouseX;
            int dy = m_lastMouseY - m_lastMouseY;
            RotateCamera(static_cast<float>(dx), static_cast<float>(dy));
        }
    }

    void InputManager::OnMouseMove(int x, int y)
    {
        if (m_mouseCaptured)
        {
            int dx = x - m_lastMouseX;
            int dy = y - m_lastMouseY;
            RotateCamera(static_cast<float>(dx), static_cast<float>(dy));
        }
        m_lastMouseX = x;
        m_lastMouseY = y;
    }

    void InputManager::OnMouseDown(int button, HWND hwnd)
    {
        if (button == MK_RBUTTON)
        {
            SetCapture(hwnd);
            m_mouseCaptured = true;
        }
    }

    void InputManager::OnMouseUp(int button)
    {
        if (button == MK_RBUTTON)
        {
            ReleaseCapture();
            m_mouseCaptured = false;
        }
    }

    void InputManager::OnKeyDown(unsigned char key)
    {
        m_keyState[key] = true;

        if (key == 'Z')
            MoveCamera(0.0f, 0.0f, 1.0f);
        if (key == 'Q')
            MoveCamera(1.0f, 0.0f, 0.0f);
        if (key == 'S')
            MoveCamera(0.0f, 0.0f, -1.0f);
        if (key == 'D')
            MoveCamera(-1.0f, 0.0f, 1.0f);
    }

    void InputManager::OnKeyUp(unsigned char key)
    {
        m_keyState[key] = false;
    }

    void InputManager::RotateCamera(float dx, float dy)
    {
        m_cameraRotation.y += dx * 0.01f;
        m_cameraRotation.x += dy * 0.01f;

        m_cameraRotation.x = std::max<float>(-DirectX::XM_PIDIV2, m_cameraRotation.x);
        m_cameraRotation.x = std::min<float>(+DirectX::XM_PIDIV2, m_cameraRotation.x);
    }

    void InputManager::MoveCamera(float dx, float dy, float dz)
    {
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_cameraRotation.x, m_cameraRotation.y, m_cameraRotation.z);
        DirectX::XMVECTOR direction = DirectX::XMVectorSet(dx, dy, dz, 0.0f);
        direction = DirectX::XMVector3TransformNormal(direction, rotationMatrix);

        m_cameraPosition.x += DirectX::XMVectorGetX(direction);
        m_cameraPosition.y += DirectX::XMVectorGetY(direction);
        m_cameraPosition.z += DirectX::XMVectorGetZ(direction);
    }
}

*/