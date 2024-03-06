#include "pch.h"
#include "InputManager.h"
#include <windows.h>

namespace ave {

	InputManager::InputManager() {

		for (KeyCode key = KeyCode::A; key <= KeyCode::Divide; key = static_cast<KeyCode>(static_cast<int>(key) + 1)) {

			switch (key) {
				keys[key] = KeyState::Up;
			}
		}

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

}


/*
ave::InputManager inputManager;

while (true) {
	inputManager.UpdateKeyStates();

	// Check the state of the 'A' key
	ave::KeyState AKeyState = inputManager.GetKeyState(ave::KeyCode::A);
	ave::KeyState ZKeyState = inputManager.GetKeyState(ave::KeyCode::Z);
	ave::KeyState EKeyState = inputManager.GetKeyState(ave::KeyCode::E);
	ave::KeyState RKeyState = inputManager.GetKeyState(ave::KeyCode::R);
	ave::KeyState TKeyState = inputManager.GetKeyState(ave::KeyCode::T);
	ave::KeyState YKeyState = inputManager.GetKeyState(ave::KeyCode::Y);
	ave::KeyState UKeyState = inputManager.GetKeyState(ave::KeyCode::U);
	ave::KeyState IKeyState = inputManager.GetKeyState(ave::KeyCode::I);
	ave::KeyState OKeyState = inputManager.GetKeyState(ave::KeyCode::O);

	// Print the state of the 'A' key
	if (AKeyState == ave::KeyState::Pressed)
		std::cout << 'A';
	if (ZKeyState == ave::KeyState::Pressed)
		std::cout << 'Z';
}
	*/