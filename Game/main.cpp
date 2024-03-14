#include <AvengersEngine.h>
#include "GameManager.h"
#include <vector>
#include <sstream>
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
    if (gameInstance->Initialize(hInstance) == false) {
        MessageBoxA(NULL, "Euh ça bug ?", "ALED", 0);
        return 0;
    }

    ave::EntityManager* manager = gameInstance->GetManager();
    ave::Entity* gameManagerEntity = manager->NewEntity();
    GameManager* gameManager = gameManagerEntity->AddComponent<GameManager>();
    gameManager->Initialize(gameInstance);
    manager->RegisterEntity(gameManagerEntity);
  
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