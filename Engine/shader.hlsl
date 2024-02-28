Texture2D gTex : register(t0);

Sampler gSampler : register(s0);

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
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXTCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
    vout.pos = mul(pos, gViewProj);
    
    vout.uv = vin.uv;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gTex.Sample(gSampler, pin.uv);
}

