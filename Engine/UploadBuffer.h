#pragma once
#include "D3DUtils.h"
#include "d3d12.h"
#include <wrl.h>

namespace ave {

    class UploadBuffer
    {
    public:
        UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer, size_t elementSize) :
            mIsConstantBuffer(isConstantBuffer), mElementByteSize(elementSize)
        {
            if (isConstantBuffer)
                mElementByteSize = D3DUtils::CalcConstantBufferByteSize(elementSize);

            auto oHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
            auto RessourceBuffer = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
            ThrowIfFailed(device->CreateCommittedResource(
                &oHeapProperties,
                D3D12_HEAP_FLAG_NONE,
                &RessourceBuffer,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&mUploadBuffer)));

            ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
        }

        UploadBuffer(const UploadBuffer& rhs) = delete;
        UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
        ~UploadBuffer()
        {
            if (mUploadBuffer != nullptr)
                mUploadBuffer->Unmap(0, nullptr);

            mMappedData = nullptr;
        }

        ID3D12Resource* Resource() const
        {
            return mUploadBuffer;
        }

        void CopyData(int elementIndex, const void* data)
        {
            memcpy(&mMappedData[elementIndex * mElementByteSize], data, mElementByteSize);
        }

    private:
        ID3D12Resource* mUploadBuffer;
        BYTE* mMappedData = nullptr;

        UINT mElementByteSize = 0;
        bool mIsConstantBuffer = false;
    };
}