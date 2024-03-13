Texture2D gTex : register(t0);
SamplerState gSampler : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPerPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
}

struct VertexIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    //Transform to homogeneous to space
    float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
    vout.pos = mul(pos, mul(gView, gProj));
    vout.uv = vin.uv;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 res = gTex.Sample(gSampler, pin.uv);
    //res.a *= sin(0 + pin.pos.x);
    return res;
}

void main(in VertexIn vin, out VertexOut vout)
{
    vout = VS(vin);
}

