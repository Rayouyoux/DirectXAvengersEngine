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
        void Initialize();

        // Methods for handling keyboard input
        int IsKeyDownI();
        char IsKeyDownC();

	};
}

