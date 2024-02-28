#pragma once
#include <d3d12.h>

namespace ave {
	class Mesh
	{
	public:
		Mesh();
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView()const;
		void Destroy();
		~Mesh();

	protected:
		
		ID3DBlob* m_poVertexBufferCPU;
		ID3DBlob* m_poIndexBufferCPU;

		ID3D12Resource* m_poVertexBufferGPU;
		ID3D12Resource* m_poIndexBufferGPU;

		UINT m_oVertexByteStride;
		UINT m_oVertexBufferByteSize;

		DXGI_FORMAT m_oIndexFormat;
		UINT m_oIndexBufferByteSize;

	};


}
