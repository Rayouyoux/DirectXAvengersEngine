#include <string>
#include <cstring>
#include <D3Dcompiler.h>
#include "pch.h"
#include "Shader.h"
#include "GraphicsHandler.h"
#include "UploadBuffer.h"
#include "Vertex.h"

namespace ave {
    Shader::Shader()
    {}

    void Shader::Begin(ID3D12GraphicsCommandList* pList) {

        //Root
        pList->SetGraphicsRootSignature(m_poRootSignature);

        //Pass
        pList->SetGraphicsRootConstantBufferView(1, m_poPass->Resource()->GetGPUVirtualAddress());

        //Pipeline
        pList->SetPipelineState(m_poPso);

        //Topology
        pList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    bool Shader::Create(BYTE* oSrc, int iSize) {

        m_poDevice = m_poGraphics->GetDevice();
        //m_poCbvHeap =  //Get heap 

        //On compile le Vertex Shader
        m_poVS = Compile(oSrc, iSize, "VS", "vs_5_0");
        if (m_poVS == nullptr) {
            Destroy();
            return false;
        }

        //On compile le Pixel Shader
        m_poPS = Compile(oSrc, iSize, "PS", "ps_5_0");
        if (m_poPS == nullptr) {
            Destroy();
            return false;
        }

        if (m_poDevice->CreateRootSignature(0, m_poSerializedRootSig->GetBufferPointer(), m_poSerializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_poRootSignature))) {
            Destroy();
            return false;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC oPestoDesc;
        ZeroMemory(&oPestoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
        oPestoDesc.InputLayout = { m_oInputLayout.data(), (UINT)m_oInputLayout.size() };
        oPestoDesc.pRootSignature = m_poRootSignature;
        oPestoDesc.VS =
        {
            reinterpret_cast<BYTE*>(m_poVS->GetBufferPointer()),
            m_poVS->GetBufferSize()
        };
        oPestoDesc.PS =
        {
            reinterpret_cast<BYTE*>(m_poPS->GetBufferPointer()),
            m_poPS->GetBufferSize()
        };
        oPestoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        oPestoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        oPestoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        oPestoDesc.SampleMask = UINT_MAX;
        oPestoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        oPestoDesc.NumRenderTargets = 1;
        oPestoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UINT;
        oPestoDesc.SampleDesc.Count = m_poGraphics->Get4xMsaaState() ? 4 : 1;
        oPestoDesc.SampleDesc.Quality = m_poGraphics->Get4xMsaaState() ? (m_poGraphics->Get4xMsaaQuality() - 1) : 0;
        oPestoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        if (m_poDevice->CreateGraphicsPipelineState(&oPestoDesc, IID_PPV_ARGS(&m_poPso)) != S_OK)
        {
            Destroy();
            return false;
        }
        return true;

    }

    void Shader::Draw(ID3D12GraphicsCommandList* pList,Mesh* pMesh,Texture* pTexture,Texture* pTexture2 ) {

        //D3D12_VERTEX_BUFFER_VIEW = //on récupere le VBV;
        //D3D12_INDEX_BUFFER_VIEW = //on récupere l'IBV;

        //if (m_iRootTexture != -1 && pTexture)
        //    pList->SetGraphicsRootDescriptorTable(m_iRootTexture, /*D3D12_GPU_DESCRIPTOR_HANDLE de la classe pTexture */ );

        //if (m_iRootTexture2 != -1 && pTexture2)
        //    pList->SetGraphicsRootDescriptorTable(m_iRootTexture2, /*D3D12_GPU_DESCRIPTOR_HANDLE de la classe pTexture */);

        //Manque du code ici pour recuperer l'index de l'objet en question
        //pList->SetGraphicsRootConstantBufferView(m_iRootObject, l'index dans le tableau de vecteur m_voObjects->Ressource()->GetGPUVirtualAdress();
        //pList->DrawIndexedInstanced(pMesh->GetIndexCount(),1,0,0,0);
        // on incremente l'index d'object
        //  et on l'ajoute au vecteur
    }

    void Shader::UpdateObject() {
        //int index =  //Get l'index de l objet;
        //m_voObjects[1]->CopyData()
    }

    ID3DBlob* Shader::Compile(BYTE* oBuffer, int iSize, std::string oEntryPoint, std::string oTarget) {
        
        ID3DBlob* poByteCode = nullptr;
#ifdef _DEBUG
        
        UINT oflags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
        ID3DBlob* poErrors = nullptr;
        HRESULT hr = S_OK;
        
        hr = D3DCompile(oBuffer, iSize, "",nullptr, nullptr, oEntryPoint.c_str(), oTarget.c_str(), oflags,0,&poByteCode,&poErrors);

        if (poErrors) {
            OutputDebugStringA((char*)poErrors->GetBufferPointer());
        }
        if (hr != S_OK) {
            return nullptr;
        }
#else
        hr = D3DCompile(oBuffer, iSize, "", nullptr, nullptr, oEntryPoint.c_str(), oTarget.c_str(), oflags, 0, &poByteCode, &poErrors);
        if (hr != S_OK) {
            return nullptr;
        }
#endif
        return poByteCode;
    }

    bool Shader::CreateRootSignature(int id) {

        CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;

        switch (id) {
            case ROOTSIGNATURE_VERTEX: {
                m_oInputLayout =
                {
                    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                };
                const int count = 2;
                CD3DX12_ROOT_PARAMETER slotRootParameters[count];
                slotRootParameters[0].InitAsConstantBufferView(0);
                slotRootParameters[1].InitAsConstantBufferView(1);
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
    }
    void Shader::AddObject() {
        m_voObjects.push_back(m_poPass);
    }
    void Shader::Destroy() {
        m_poPso->Release();
        delete m_poPass;
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

    void Shader::End() {
        
    }

    Shader::~Shader() {

    }
}
