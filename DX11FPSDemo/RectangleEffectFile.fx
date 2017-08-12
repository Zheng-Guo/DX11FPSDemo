cbuffer cbPerObject
{
    float4x4 W;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color: COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor: COLOR)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, W);

    output.Color=inColor;

    return output;
}

float4 PS(VS_OUTPUT input):SV_Target
{
    return input.Color;
}