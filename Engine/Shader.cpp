#include <string>
#include <cstring>
#include <D3Dcompiler.h>
#include "pch.h"
#include "Shader.h"
#include "GraphicsHandler.h"
#include "UploadBuffer.h"
#include "Vertex.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include <iostream>
#include "ConstantsStruct.h"

namespace ave {
    
    Shader::Shader(){
        m_poGraphics = nullptr;
        m_poDevice = nullptr;
        m_poCbvHeap = nullptr;
        m_poSerializedRootSig = nullptr;
        m_poPass = nullptr;
        m_poObject = nullptr;
        m_poRootSignature = nullptr;
        m_poVS = nullptr;
        m_poPS = nullptr;

        m_iIdRootSignature = 0;
        m_iRootTexture = 0;
        m_iRootTexture2 = 0;
        m_iRootObject = 0;
        m_iRootPass = 0;
        m_iTextureCount = 0;
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

    int Shader::GetRootTexture() {
        return m_iRootTexture;
    }

    int Shader::GetRootTexture2() {
        return m_iRootTexture2;
    }



    ID3D12RootSignature* Shader::GetRootSignature() {
        return m_poRootSignature;
    }

  


    bool Shader::CreateShader(GraphicsHandler* poGraphicsHandler, Camera* poCamera, int id) {
        m_poCamera = poCamera;
        m_iIndexObject = 0;
        m_poDevice = poGraphicsHandler->GetDevice();
        //m_poCbvHeap =  //Get heap 

        std::wstring shader;
        switch (id) {
        case 1:
            shader = L"ROOTSIGNATURE_VERTEX_COLOR";
            break;
        case 2:
            shader = L"ROOTSIGNATURE_VERTEX_UV";
            break;
        }

        //On compile le Vertex Shader
        m_poVS = CompileShader(L"..\\Engine\\Shader\\shader_" + shader + L".hlsl", "VS", "vs_5_0");
        if (m_poVS == nullptr) {
            Destroy();
            return false;
        }

        //On compile le Pixel Shader
        m_poPS = CompileShader(L"..\\Engine\\Shader\\shader_" + shader + L".hlsl", "PS", "ps_5_0");
        if (m_poPS == nullptr) {
            Destroy();
            return false;
        }

        CreateRootSignature(id);

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
    
    void Shader::Draw(D3D12_VERTEX_BUFFER_VIEW& vertexBuffer, D3D12_INDEX_BUFFER_VIEW& indexBuffer, UINT indexCount, UploadBuffer<ObjectConstants>* poBuffer, Texture* oTexture) {
        ID3D12GraphicsCommandList* poList = GraphicsHandler::GetCommandList();

        ////Root
        poList->SetGraphicsRootSignature(GetRootSignature());

        poList->SetGraphicsRootConstantBufferView(GetRootPass(), m_poCamera->m_poBuffer->Resource()->GetGPUVirtualAddress());
        ////Pipeline
        poList->SetPipelineState(GetPso("transparent"));

        ////Topology
        poList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        poList->IASetVertexBuffers(0, 1, &vertexBuffer);
        poList->IASetIndexBuffer(&indexBuffer);

        if (GetRootTexture() != -1 && oTexture)
        {
            //CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_poTextures->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
            //auto test = std::distance(m_poTextures->GetTexture()->begin(), m_poTextures->GetTexture()->find(oName));
            //std::distance(m_poTextures->GetTexture()->begin(), m_poTextures->GetTexture()->find(oName)) / 2
            //tex.Offset(std::distance(m_poTextures->GetTexture()->begin(), m_poTextures->GetTexture()->find(oName))/2, *m_poTextures->GetDescriptorSize());

            poList->SetGraphicsRootDescriptorTable(GetRootTexture(), *oTexture->GetDescriptorGpuHandle());
        }

        poList->SetGraphicsRootConstantBufferView(GetRootObject(), poBuffer->Resource()->GetGPUVirtualAddress());

        poList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
    }

    void Shader::Draw(Mesh* pMesh, UploadBuffer<ObjectConstants>* poBuffer, Texture* oTexture) {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView = pMesh->VertexBufferView();
        D3D12_INDEX_BUFFER_VIEW indexBufferView = pMesh->IndexBufferView();
        Draw(vertexBufferView, indexBufferView, pMesh->GetIndexCount(), poBuffer, oTexture);
    }

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

            // R�cup�rer la taille du message d'erreur
            SIZE_T size = poErrors->GetBufferSize();

            // Imprimer le message d'erreur dans la sortie du d�bogueur
            OutputDebugStringA(static_cast<LPCSTR>(pErrorMsg));

            // Imprimer le message d'erreur dans la console (pour une application de console)
            std::cerr << static_cast<const char*>(pErrorMsg) << std::endl;

            // Lib�rer la m�moire du buffer d'erreur
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
            }
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
            }
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

                auto staticSamplers = GetStaticSamplers();
                    
                rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)staticSamplers.size(), staticSamplers.data(),
                    D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
                ID3DBlob* error = nullptr;

                //a redeplacer
                HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &m_poSerializedRootSig, &error);

                if (error != nullptr)
                {
                    ::OutputDebugStringA((char*)error->GetBufferPointer());
                    return false;
                }
                return true;
                break;
            }
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

                auto staticSamplers = GetStaticSamplers();

                rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
                
                break;
            }
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

                auto staticSamplers = GetStaticSamplers();

                rootSigDesc = CD3DX12_ROOT_SIGNATURE_DESC(count, slotRootParameters, (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
                break;
            }
            default: {
                Destroy();
                return false;
            }
        }
        ID3DBlob* error = nullptr;
        HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &m_poSerializedRootSig, &error);

        if (FAILED(hr))
        {
            if (error != nullptr)
            {
                ::OutputDebugStringA((char*)error->GetBufferPointer());
            }
            else
            {
                // Afficher le code d'erreur directement
                ::OutputDebugStringA("Failed to serialize root signature without error details.\n");
            }

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

    void Shader::End() {
        
    }

    std::vector<CD3DX12_STATIC_SAMPLER_DESC> Shader::GetStaticSamplers()
    {
        std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplers;

        samplers.emplace_back(
            0,                                  // Register
            D3D12_FILTER_MIN_MAG_MIP_LINEAR,    // Filter
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressU
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressV
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressW
            0.0f,                               // MipLODBias
            8,                                  // MaxAnisotropy
            D3D12_COMPARISON_FUNC_LESS_EQUAL,   // ComparisonFunc
            D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE, // BorderColor
            0.0f,                               // MinLOD
            D3D12_FLOAT32_MAX,                  // MaxLOD
            D3D12_SHADER_VISIBILITY_ALL);

        return samplers;
    }

    Shader::~Shader() {
        Destroy();
    }

}
