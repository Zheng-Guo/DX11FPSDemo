cbuffer cbPerObject
{
    float4x4 WVP;
};

struct Light
{
    float3 dir;
    float4 ambient;
};

cbuffer cbPerFrame
{
    Light light;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color: COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor: COLOR)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);

    output.Color=inColor;

    return output;
}

float4 PS(VS_OUTPUT input):SV_Target
{
    return input.Color*light.ambient;
//return float4(1.f,0.f,0.f,1.f);
}