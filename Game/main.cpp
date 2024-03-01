#include "AvengersEngine.h"
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtMemState memStateInit;
    _CrtMemCheckpoint(&memStateInit);
#endif

    ave::AvengersEngine* gameInstance = ave::AvengersEngine::Create();
    if (gameInstance->Initialize(hInstance) == false)
        return 0;

    int res = gameInstance->Run();
#ifdef _DEBUG
    _CrtMemState memStateEnd, memStateDiff;
    _CrtMemCheckpoint(&memStateEnd);
    if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
    {
        MessageBoxA(NULL, "MEMORY LEAKS", "DISCLAIMER", 0);
    }
#endif 
    return res;
}