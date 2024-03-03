#pragma once

namespace ave {

	class InputManager
	{
    public:

        // Constructor
        InputManager();

        // Deconstructor
        ~InputManager();

        // Main methods
        void Initialize(HWND hwnd);
        void Update();

        // Methods for handling mouse input
        void OnMouseMove(int x, int y);
        void OnMouseDown(int button, HWND hwnd);
        void OnMouseUp(int button);

        // Methods for handling keyboard input
        void OnKeyDown(unsigned char key);
        void OnKeyUp(unsigned char key);

    private:

        // Camera Control Methods
        void RotateCamera(float dx, float dy);
        void MoveCamera(float dx, float dy, float dz);

    private:

        // Camera Control Variables
        DirectX::XMFLOAT3 m_cameraPosition;
        DirectX::XMFLOAT3 m_cameraRotation;

        // Mouse 
        bool m_mouseCaptured;
        int m_lastMouseX;
        int m_lastMouseY;

        // Keyboard
        bool m_keyState[256];

	};
}

