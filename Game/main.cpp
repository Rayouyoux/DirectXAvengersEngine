#include "AvengersEngine.h"
#include "ObjectPooler.h"
#include <vector>
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

using namespace ave::ObjectPooling;

class Particle : public IPullable {
    virtual void OnInstantiation() override {
    }

    virtual void OnAcquire() override {
    }

    virtual void OnRelease() override {
    }
};

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

    ObjectPool* myPool = ObjectPool::Create();
    myPool->Initialize<Particle>(10);

    for (int i = 0; i < 8; i++) {
        myPool->AcquireObject<Particle>();
    }

    int count = myPool->CountActive();

    DirectX::XMVECTORF32 color = (count == 8) ? DirectX::Colors::Green : DirectX::Colors::Orange;
    gameInstance->GetGraphics()->SetFillColor(color);

    myPool->Release();
  
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