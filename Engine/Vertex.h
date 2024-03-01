#pragma once

#include <DirectXMath.h>

namespace ave {
#define ROOTSIGNATURE_VERTEX				0
#define ROOTSIGNATURE_VERTEX_COLOR			1
#define ROOTSIGNATURE_VERTEX_UV				2
#define ROOTSIGNATURE_VERTEX_COLOR_UV		3
#define ROOTSIGNATURE_VERTEX_2UV			4

	using namespace DirectX;

	struct VERTEX {

		XMFLOAT3 pos;

	};
	struct VERTEX_COLOR {

		XMFLOAT3 pos;
		XMFLOAT4 color;

	};
	struct VERTEX_UV {

		XMFLOAT3 pos;
		XMFLOAT2 uv;

	};
	struct VERTEX_COLOR_UV {

		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;

	};

	struct VERTEX_2UV {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
		XMFLOAT2 uv2;
	};
}
