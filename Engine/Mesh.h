#pragma once
#include <d3d12.h>
#include <iostream>
#include <string>

namespace ave {
	class Mesh
	{
	public:
		Mesh();

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const;

		UINT GetIndexCount();
		bool BuildBoxGeometry(ID3D12Device* poDevice, ID3D12GraphicsCommandList* poCommandList, std::string nameShape);
		void Destroy();
		~Mesh();

	protected:
		
		ID3DBlob* m_poVertexBufferCPU;
		ID3DBlob* m_poIndexBufferCPU;

		ID3D12Resource* m_poVertexBufferGPU;
		ID3D12Resource* m_poIndexBufferGPU;

		ID3D12Resource* m_poVertexBufferUploader;
		ID3D12Resource* m_poIndexBufferUploader;

		UINT m_oVertexByteStride;
		UINT m_oVertexBufferByteSize;

		DXGI_FORMAT m_oIndexFormat;
		UINT m_oIndexBufferByteSize;

		UINT m_oIndexCount;
		

	};


}
