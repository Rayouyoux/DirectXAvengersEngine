Texture2D gTex : register(t0);
SamplerState gSampler : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPerPass : register(b1)
{
    float4x4 gViewProj;
}

struct VertexIn
{
    float3 pos : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    //Transform to homogeneous to space
    float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
    vout.pos = mul(pos, gViewProj);
    
    //vout.color = vin.color;
    vout.uv = vin.uv;
    
    return vout;
}

void main(in VertexIn vin, out VertexOut vout)
{
    vout = VS(vin);
}