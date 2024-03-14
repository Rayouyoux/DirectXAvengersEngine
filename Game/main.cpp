#include <AvengersEngine.h>
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

    //ave::Entity* gameManagerEntity = gameInstance->AccessEntityManager()->NewEntity();
    //GameManager* gameManager = gameManagerEntity->AddComponent<GameManager>();
    //gameManager->Initialize(*gameInstance);
    //gameInstance->AccessEntityManager()->RegisterEntity(gameManagerEntity);

    ave::EntityManager* manager = gameInstance->GetGraphics()->m_poEntityManager;

    ave::Entity* poCubeEntity = manager->NewEntity();
    ave::Entity* poCubeEntity2 = manager->NewEntity();

    XMVECTOR posCube = XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f);
    poCubeEntity->m_poTransform->SetVectorPosition(&posCube);

    ave::Entity* poCamera = manager->GetMainCamera()->GetEntity();
    XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
    poCamera->m_poTransform->SetVectorPosition(&pos);

    XMVECTOR direction = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
    poCamera->m_poTransform->LookTo(&direction);

    manager->NewTexture("bricks", "../Engine/Textures/bricks.dds");
    manager->NewTexture("image", "../Engine/Textures/image.dds");

    ave::MeshRenderer* poMeshRenderer = poCubeEntity->AddComponent<ave::MeshRenderer>();
    poMeshRenderer->SetMesh(manager->GetMesh("pyramid"));
    poMeshRenderer->SetShader(manager->GetShader("Color"));
    //poMeshRenderer->SetTexture(manager->GetTexture("bricks"));
  /*  XMVECTOR test = XMVectorSet(0.0f, 255.0f, 255.0f, 1.0f);
    poMeshRenderer->SetColor(&test);*/

    ave::MeshRenderer* poMeshRenderer2 = poCubeEntity2->AddComponent<ave::MeshRenderer>();
    poMeshRenderer2->SetMesh(manager->GetMesh("skyboxTexture"));
    poMeshRenderer2->SetShader(manager->GetShader("Texture"));
    poMeshRenderer2->SetTexture(manager->GetTexture("image"));
    XMVECTOR scale = XMVectorSet(4.5f, 4.5f, 4.5f, 4.5f);
    poCubeEntity2->m_poTransform->Scale(&scale);

    ave::UI::SpriteRenderer* spriteRenderer = poCubeEntity->AddComponent<ave::UI::SpriteRenderer>();
    XMVECTOR pixelPos = XMVectorSet(0, 0, 0, 0);
    XMVECTOR pixelSize = XMVectorSet(100, 100, 0, 0);
    spriteRenderer->SetPixelPosition(pixelPos);
    spriteRenderer->SetPixelSize(pixelSize);
    spriteRenderer->SetMesh(manager->GetMesh("planeTexture"));
    spriteRenderer->SetShader(manager->GetShader("Texture"));

    manager->RegisterEntity(poCubeEntity);

    manager->RegisterEntity(poCubeEntity2);

    int res = gameInstance->Run();

#ifdef _DEBUG
    _CrtMemState memStateEnd, memStateDiff;
    _CrtMemCheckpoint(&memStateEnd);
    if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
    {
        MessageBoxA(NULL, "Memory leaks", "DISCLAIMER", 0);
    }
#endif 
    return res;
}