#pragma once

#include <map>

namespace ave {

	enum class KeyState {
		Down,
		Up,
		Pressed,
		Released
	};

	enum class KeyCode {
		// Alphabetical keys
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		// Number keys
		Num0 = 0x30,
		Num1 = 0x31,
		Num2 = 0x32,
		Num3 = 0x33,
		Num4 = 0x34,
		Num5 = 0x35,
		Num6 = 0x36,
		Num7 = 0x37,
		Num8 = 0x38,
		Num9 = 0x39,

		// Function keys
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,

		// Other keys
		Space = 0x20,
		Enter = 0x0D,
		Backspace = 0x08,
		Tab = 0x09,
		CapsLock = 0x14,
		Escape = 0x1B,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftControl = 0xA2,
		RightControl = 0xA3,
		LeftAlt = 0xA4,
		RightAlt = 0xA5,
		LeftWindows = 0x5B,
		RightWindows = 0x5C,
		Insert = 0x2D,
		Delete = 0x2E,
		Home = 0x24,
		End = 0x23,
		PageUp = 0x21,
		PageDown = 0x22,
		UpArrow = 0x26,
		DownArrow = 0x28,
		LeftArrow = 0x25,
		RightArrow = 0x27,
		PrintScreen = 0x2C,
		ScrollLock = 0x91,
		Pause = 0x13,
		NumLock = 0x90,
		NumPad0 = 0x60,
		NumPad1 = 0x61,
		NumPad2 = 0x62,
		NumPad3 = 0x63,
		NumPad4 = 0x64,
		NumPad5 = 0x65,
		NumPad6 = 0x66,
		NumPad7 = 0x67,
		NumPad8 = 0x68,
		NumPad9 = 0x69,
		Multiply = 0x6A,
		Add = 0x6B,
		Subtract = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F
	};

	class InputManager {
	private:
		std::map<KeyCode, KeyState> keys;

	public:
		InputManager();

		KeyState GetKeyState(KeyCode key);
		bool GetKeyDown(KeyCode key);
		bool GetKeyUp(KeyCode key);
		void UpdateKeyStates();
	};
}
