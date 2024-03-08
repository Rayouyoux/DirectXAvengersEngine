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
    ave::EntityManager* gameInstance = ave::EntityManager::Create();
    if (gameInstance->Initialize(hInstance) == false) {
        MessageBoxA(NULL, "Euh ça bug ?", "ALED", 0);
        return 0;
    }

    ave::Shader* poShader = gameInstance->NewShader();
    ave::Mesh* poMesh = gameInstance->NewMesh();

    ave::Entity* poCubeEntity = gameInstance->NewEntity();
    ave::Entity* poCubeEntity2 = gameInstance->NewEntity();

    XMVECTOR posCube = XMVectorSet(5.0f, 0.0f, 0.0f, 0.0f);
    poCubeEntity->m_poTransform->SetVectorPosition(&posCube);

    ave::Entity* poCamera = gameInstance->GetMainCamera();
    XMVECTOR pos = XMVectorSet(5.0f, 0.0f, 4.0f, 0.0f);
    poCamera->m_poTransform->SetVectorPosition(&pos);

    XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
    poCamera->m_poTransform->LookTo(&direction);

    ave::MeshRenderer* poMeshRenderer = poCubeEntity->AddComponent<ave::MeshRenderer>();
    poMeshRenderer->SetMesh(poMesh);
    poMeshRenderer->SetShader(poShader);

    ave::MeshRenderer* poMeshRenderer2 = poCubeEntity2->AddComponent<ave::MeshRenderer>();
    poMeshRenderer2->SetMesh(poMesh);
    poMeshRenderer2->SetShader(poShader);

    gameInstance->RegisterEntity(poCubeEntity);
    gameInstance->RegisterEntity(poCubeEntity2);
  
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