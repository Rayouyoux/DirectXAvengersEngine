Texture2D gTex : register(t0);
SamplerState gSampler : register(s0);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4 colorPass;
};

cbuffer cbPerPass : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
}

bool IsFloat4Empty(float4 fColor)
{
    return (fColor.x == 0.0f && fColor.y == 0.0f && fColor.z == 0.0f && fColor.w == 0.0f);
}

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    //Transform to homogeneous to space
    float4 pos = mul(float4(vin.pos, 1.0f), gWorld);
    vout.pos = mul(pos, mul(gView, gProj));
    if (IsFloat4Empty(colorPass))
    {
        vout.color = vin.color;
    }
    else
    {
        vout.color = colorPass;
    }
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.color;
}

void main(in VertexIn vin, out VertexOut vout)
{
    vout = VS(vin);
}

