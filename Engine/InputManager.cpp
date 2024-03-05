#include "pch.h"
#include "InputManager.h"
#include <conio.h>
#include <__msvc_chrono.hpp>
#include <thread>

namespace ave {

    InputManager::InputManager()
    {}

    InputManager::~InputManager()
    {}

    void InputManager::Initialize()
    {}

    int InputManager::IsKeyDownI()
    {
        if (_kbhit()) {
            char key = _getch();
            return static_cast<int>(key);
        }
        return 0;
    }

    char InputManager::IsKeyDownC()
    {
        if (_kbhit()) {
            char key = _getch();
            return key;
        }
        return;
    }

}