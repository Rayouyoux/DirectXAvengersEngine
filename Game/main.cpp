#include "AvengersEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        ave::AvengersEngine* gameInstance = ave::AvengersEngine::Create();
        if (gameInstance->Initialize(hInstance) == false)
            return 0;

        return gameInstance->Run();
    }
    catch (ave::DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}