#include "AvengersEngine.h"
#include "ObjectPooler.h"
#include <vector>

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
#endif
    ave::AvengersEngine* gameInstance = ave::AvengersEngine::Create();
    if (gameInstance->Initialize(hInstance) == false)
        return 0;

    ObjectPool* myPool = ObjectPool::Create();
    myPool->Initialize<Particle>(10);

    for (int i = 0; i < 8; i++) {
        myPool->AcquireObject<Particle>();
    }

    int count = myPool->CountActive();

    DirectX::XMVECTORF32 color = (count == 8) ? DirectX::Colors::Green : DirectX::Colors::Orange;
    gameInstance->GetGraphics()->SetFillColor(color);

    myPool->Release();
    
    return gameInstance->Run();
}