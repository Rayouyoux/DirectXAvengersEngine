#include <string>
#include <cstring>
#include <D3Dcompiler.h>
#include "pch.h"
#include "Shader.h"
#include "GraphicsHandler.h"
#include "UploadBuffer.h"
#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include <iostream>
#include "ConstantsStruct.h"

namespace ave {


    Shader::Shader() {

    }
    ID3D12PipelineState* Shader::GetPso(const char* blend) {
        return m_dPSOs[blend];
    }

    UINT Shader::GetRootObject() {
        return m_iRootObject;
    }

    UINT Shader::GetRootPass() {
        return m_iRootPass;
    }

    /*D3D12_GPU_VIRTUAL_ADDRESS Shader::GetVirtualAdress() {
        
        D3D12_GPU_VIRTUAL_ADDRESS oVirtualAdress = m_voObjects[m_iIndexObject]->Resource()->GetGPUVirtualAddress();
        m_iIndexObject++;
        return oVirtualAdress;
    }*/

    ID3D12RootSignature* Shader::GetRootSignature() {
        return m_poRootSignature;
    }

    UploadBuffer<PassConstants>* Shader::GetPass() {
        return m_poPass;
    }
    //void Shader::Start(ID3D12GraphicsCommandList* pList, ID3D12Device* poDevice) {

    //    //Root
    //    pList->SetGraphicsRootSignature(m_poRootSignature);

    //    m_poPass = new UploadBuffer(poDevice, 100, false, sizeof(Pass));
    //    //Pass
    //    pList->SetGraphicsRootConstantBufferView(1, m_poPass->Resource()->GetGPUVirtualAddress());

    //    //Create((BYTE*)L"shader.hlsl", sizeof(BYTE*));

    //    //Pipeline
    //    pList->SetPipelineState(m_poPso);

    //    //Topology
    //    pList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //}

    //A appeler dans l'init
    bool Shader::CreateShader(GraphicsHandler* poGraphicsHandler) {
        m_iIndexObject = 0;
        m_poDevice = poGraphicsHandler->GetDevice();
        CreateUploadBuffer();
        CreateRootSignature(1);
        //m_poCbvHeap =  //Get heap 

        //On compile le Vertex Shader
        m_poVS = CompileShader(L"..\\Engine\\Shaders\\transparent_shader.hlsl", "VS", "vs_5_0");
        if (m_poVS == nullptr) {
            Destroy();
            return false;
        }

        //On compile le Pixel Shader
        m_poPS = CompileShader(L"..\\Engine\\Shaders\\transparent_shader.hlsl", "PS", "ps_5_0");
        if (m_poPS == nullptr) {
            Destroy();
            return false;
        }


        if (m_poDevice->CreateRootSignature((UINT)0, m_poSerializedRootSig->GetBufferPointer(), m_poSerializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_poRootSignature))) {
            Destroy();
            return false;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC oOpaquePSO;
        ZeroMemory(&oOpaquePSO, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
        oOpaquePSO.InputLayout = { m_oInputLayout.data(), (UINT)m_oInputLayout.size() };
        oOpaquePSO.pRootSignature = m_poRootSignature;
        oOpaquePSO.VS =
        {
            reinterpret_cast<BYTE*>(m_poVS->GetBufferPointer()),
            m_poVS->GetBufferSize()
        };
        oOpaquePSO.PS =
        {
            reinterpret_cast<BYTE*>(m_poPS->GetBufferPointer()),
            m_poPS->GetBufferSize()
        };
        oOpaquePSO.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        oOpaquePSO.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        oOpaquePSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        oOpaquePSO.SampleMask = UINT_MAX;
        oOpaquePSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        oOpaquePSO.NumRenderTargets = 1;
        oOpaquePSO.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        oOpaquePSO.SampleDesc.Count = poGraphicsHandler->Get4xMsaaState() ? 4 : 1;
        oOpaquePSO.SampleDesc.Quality = poGraphicsHandler->Get4xMsaaState() ? (poGraphicsHandler->Get4xMsaaQuality() - 1) : 0;
        oOpaquePSO.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        if (m_poDevice->CreateGraphicsPipelineState(&oOpaquePSO, IID_PPV_ARGS(&m_dPSOs["opaque"])) != S_OK)
        {
            Destroy();
            return false;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC oTransparentPSO = oOpaquePSO;

        D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
        transparencyBlendDesc.BlendEnable = true;
        transparencyBlendDesc.LogicOpEnable = false;
        transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
        transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
        transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
        transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
        transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        oTransparentPSO.BlendState.RenderTarget[0] = transparencyBlendDesc;
        if (m_poDevice->CreateGraphicsPipelineState(
            &oTransparentPSO, IID_PPV_ARGS(&m_dPSOs["transparent"]))) {
            Destroy();
            return false;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC oParticlePSO = oOpaquePSO;

        D3D12_RENDER_TARGET_BLEND_DESC particleBlendDesc;
        particleBlendDesc.BlendEnable = true;
        particleBlendDesc.LogicOpEnable = false;
        particleBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        particleBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        particleBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
        particleBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
        particleBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
        particleBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        particleBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
        particleBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        
        oParticlePSO.BlendState.RenderTarget[0] = particleBlendDesc;
        if (m_poDevice->CreateGraphicsPipelineState(
            &oParticlePSO, IID_PPV_ARGS(&m_dPSOs["particle"]))) {
            Destroy();
            return false;
        }

        return true;
    }

    //A appeler dans l'init
    void Shader::CreateUploadBuffer() {
        m_poPass = new UploadBuffer<PassConstants> (m_poDevice, 1, true);
        //m_poObject = new UploadBuffer<ObjectConstants>(m_poDevice, 1, true);
        /*AddObject();
        AddObject();*/
    }


    void Shader::UpdatePass(PassConstants data) {
        if (m_poPass) {
            m_poPass->CopyData(0, data);
        }
    }

  /*  void Shader::UpdateObject(ObjectConstants data) {
        if (m_poObject)
        {
            m_poObject->CopyData(0, data);
        }
    }

    void Shader::UpdateObject(ObjectConstants data, int index) {
        m_voObjects[index]->CopyData(0, data);
    }*/
    

 //   void Shader::Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2 ) {

 //       D3D12_VERTEX_BUFFER_VIEW vbv = pMesh->VertexBufferView();
 //       D3D12_INDEX_BUFFER_VIEW ibv = pMesh->IndexBufferView();

 ///*       if (m_iRootTexture != -1 && pTexture)
 //           pList->SetGraphicsRootDescriptorTable(m_iRootTexture, );*/

 //       //if (m_iRootTexture2 != -1 && pTexture2)
 //       //    pList->SetGraphicsRootDescriptorTable(m_iRootTexture2, /*D3D12_GPU_DESCRIPTOR_HANDLE de la classe pTexture */);

 //       //Manque du code ici pour recuperer l'index de l'objet en question
 //       //pList->SetGraphicsRootConstantBufferView(m_iRootObject, l'index dans le tableau de vecteur m_voObjects->Ressource()->GetGPUVirtualAdress();
 //       //pList->DrawIndexedInstanced(pMesh->GetIndexCount(),1,0,0,0);
 //       // on incremente l'index d'object
 //       //  et on l'ajoute au vecteur
 //   }

    //void Shader::UpdateObject() {
    //    //int index =  //Get l'index de l objet;
    //    //m_voObjects[1]->CopyData()
    //}

    ID3DBlob* Shader::CompileShader(const std::wstring& oBuffer, const std::string& oEntryPoint,const std::string& oTarget) {
        
        UINT oflags = 0;
#ifdef _DEBUG
        oflags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#endif
        ID3DBlob* poByteCode = nullptr;
        ID3DBlob* poErrors;
        HRESULT hr = S_OK;
        hr = D3DCompileFromFile(oBuffer.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            oEntryPoint.c_str(), oTarget.c_str(), oflags, 0, &poByteCode, &poErrors);


        if (poErrors !=nullptr) {
            LPCVOID pErrorMsg = poErrors->GetBufferPointer();

            // Récupérer la taille du message d'erreur
            SIZE_T size = poErrors->GetBufferSize();

            // Imprimer le message d'erreur dans la sortie du débogueur
            OutputDebugStringA(static_cast<LPCSTR>(pErrorMsg));

            // Imprimer le message d'erreur dans la console (pour une application de console)
            std::cerr << static_cast<const char*>(pErrorMsg) << std::endl;

            // Libérer la mémoire du buffer d'erreur
            poErrors->Release();
            OutputDebugStringA((char*)poErrors->GetBufferPointer());
        }
        if (hr != S_OK) {
            return nullptr;
        }
        return poByteCode;
    }

    bool Shader::CreateRootSignature(int id) {
        m_iIdRootSignature = id;
        m_iTextureCount = 0;
        m_iRootTexture = -1;
        m_iRootTexture2 = -1;
        m_iRootObject = 0;
        m_iRootPass = 1;

        CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;

        switch (id) {
            case ROOTSIGNATURE_VERTEX: {
                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                };
                const int count = 2;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsConstantBufferView(0); //b0 for object
                slotRootParameters[1].InitAsConstantBufferView(1); //b1 for pass
                rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
                break;
            };
            case ROOTSIGNATURE_VERTEX_COLOR: {
                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

                };
                const int count = 2;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsConstantBufferView(0);
                slotRootParameters[1].InitAsConstantBufferView(1);
                rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
                break;
            };
            case ROOTSIGNATURE_VERTEX_UV: {
                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

                };
                CD3DX12_DESCRIPTOR_RANGE texTable;
                texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

                const int count = 3;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsDescriptorTable(1,&texTable,D3D12_SHADER_VISIBILITY_PIXEL);
                slotRootParameters[1].InitAsConstantBufferView(0);
                slotRootParameters[2].InitAsConstantBufferView(1);
                m_iTextureCount = 1;
                m_iRootTexture = 0;
                m_iRootObject = 1;
                m_iRootPass = 2;

                //rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)SGraphics::I()->GetStaticSamplerCount(), SGraphics::I()->GetStaticSamplerCount());
                break;
            };
            case ROOTSIGNATURE_VERTEX_COLOR_UV: {

                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0 , 28 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

                };
                CD3DX12_DESCRIPTOR_RANGE texTable;
                texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

                const int count = 3;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
                slotRootParameters[1].InitAsConstantBufferView(0);
                slotRootParameters[2].InitAsConstantBufferView(1);
                m_iTextureCount = 1;
                m_iRootTexture = 0;
                m_iRootObject = 1;
                m_iRootPass = 2;

                //rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)SGraphics::I()->GetStaticSamplerCount(), SGraphics::I()->GetStaticSamplerCount());
                break;
            };
            case ROOTSIGNATURE_VERTEX_2UV: {

                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD",1, DXGI_FORMAT_R32G32_FLOAT, 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

                };
                CD3DX12_DESCRIPTOR_RANGE texTable;
                texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
                CD3DX12_DESCRIPTOR_RANGE texTable2;
                texTable2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

                const int count = 4;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
                slotRootParameters[1].InitAsDescriptorTable(1, &texTable2, D3D12_SHADER_VISIBILITY_PIXEL);
                slotRootParameters[2].InitAsConstantBufferView(0);
                slotRootParameters[3].InitAsConstantBufferView(1);
                m_iTextureCount = 2;
                m_iRootTexture = 0;
                m_iRootTexture2 = 1;
                m_iRootObject = 2;
                m_iRootPass = 3;

                //rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)SGraphics::I()->GetStaticSamplerCount(), SGraphics::I()->GetStaticSamplerCount());
                break;
            };
            default: {
                Destroy();
                return false;
            }
        }
        if (D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &m_poSerializedRootSig, nullptr)!= S_OK) {
            return false;
        }
    }

    ID3D12Device* Shader::GetDevice() {
        return m_poDevice;
    }

    void Shader::Destroy() {
        for (const auto& pair : m_dPSOs)
            pair.second->Release();

        delete m_poPass;
        delete m_poObject;
        for (int i = 0; i < m_voObjects.size(); i++) {
            delete m_voObjects[i];
        }
        m_voObjects.clear();
        m_poRootSignature->Release();
        m_poSerializedRootSig->Release();
        m_poVS->Release();
        m_poPS->Release();

        m_poDevice = nullptr;
    }

    void Shader::ResetIndexObject()
    {
        m_iIndexObject = 0;
    }

    void Shader::End() {
        
    }

    Shader::~Shader() {
        Destroy();
    }
}
