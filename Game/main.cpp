#include "AvengersEngine.h"
//#include "ObjectPooler.h"
#include <vector>
#include <sstream>
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

//using namespace ave::ObjectPooling;
//
//class Particle : public IPullable {
//    virtual void OnInstantiation() override {
//    }
//
//    virtual void OnAcquire() override {
//    }
//
//    virtual void OnRelease() override {
//    }
//};

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

    ave::EntityManager* manager = gameInstance->GetGraphics()->m_poEntityManager;

    ave::Entity* poCubeEntity = manager->NewEntity();
    ave::Entity* poCubeEntity2 = manager->NewEntity();

    XMVECTOR posCube = XMVectorSet(10.0f, 0.0f, 4.0f, 0.0f);
    poCubeEntity->m_poTransform->SetVectorPosition(&posCube);

    ave::Entity* poCamera = manager->GetMainCamera()->GetEntity();
    XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
    poCamera->m_poTransform->SetVectorPosition(&pos);

    XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
    poCamera->m_poTransform->LookTo(&direction);

    ave::MeshRenderer* poMeshRenderer = poCubeEntity->AddComponent<ave::MeshRenderer>();
    poMeshRenderer->SetMesh(manager->GetMesh());
    poMeshRenderer->SetShader(manager->GetShader());

    poCubeEntity->AddComponent<ave::Collider>();

    ave::MeshRenderer* poMeshRenderer2 = poCubeEntity2->AddComponent<ave::MeshRenderer>();
    poMeshRenderer2->SetMesh(manager->GetMesh());
    poMeshRenderer2->SetShader(manager->GetShader());

    poCubeEntity2->AddComponent<ave::Collider>();

    manager->RegisterEntity(poCubeEntity);
    manager->RegisterEntity(poCubeEntity2);
  
    int res = gameInstance->Run();

#ifdef _DEBUG
    _CrtMemState memStateEnd, memStateDiff;
    _CrtMemCheckpoint(&memStateEnd);
    if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
    {
        MessageBoxA(NULL, "UWU", "DISCLAIMER", 0);
    }
#endif 
    return res;
}