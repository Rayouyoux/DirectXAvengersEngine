#include "pch.h"
#include "InputManager.h"


namespace ave {

	InputManager::InputManager() {
		m_oWindow = nullptr;

		for (KeyCode key = KeyCode::A; key <= KeyCode::Divide; key = (KeyCode)((int)key + 1)) {
			keys[key] = KeyState::Up;
		}

	}

	InputManager::~InputManager() {

	}

	void InputManager::Init(HWND oWindow) {
		m_oWindow = oWindow;
	}

	KeyState InputManager::GetKeyState(KeyCode key) {
		return keys[key];
	}

	bool InputManager::GetKeyDown(KeyCode key) {
		return keys[key] == KeyState::Pressed || keys[key] == KeyState::Down;
	}

	bool InputManager::GetKeyUp(KeyCode key) {
		return keys[key] == KeyState::Released;
	}

	void InputManager::UpdateKeyStates() {
		for (auto& pair : keys) {
			KeyCode key = pair.first;
			KeyState& keyObj = pair.second;
			bool down = GetAsyncKeyState((int)key);

			KeyState prevState = keyObj;

			if (down) {
				if (prevState == KeyState::Pressed || prevState == KeyState::Down) {
					keyObj = KeyState::Down;
				}
				else if (prevState == KeyState::Released || prevState == KeyState::Up) {
					keyObj = KeyState::Pressed;
				}
			}
			else {
				if (prevState == KeyState::Pressed || prevState == KeyState::Down) {
					keyObj = KeyState::Released;
				}
				else if (prevState == KeyState::Released || prevState == KeyState::Up) {
					keyObj = KeyState::Up;
				}
			}
		}
	}

	POINT InputManager::MousePos() {
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		ScreenToClient(m_oWindow, &currentMousePos);
		return currentMousePos;
	}


}
