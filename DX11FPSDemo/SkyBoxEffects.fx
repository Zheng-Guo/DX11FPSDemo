cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
};

//Texture2D ObjTexture;
SamplerState ObjSamplerState;
TextureCube Sky;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;

float3 PosL : POSITION;
};

VS_OUTPUT VS(float4 inPos : POSITION)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);
output.PosL=inPos;

    return output;
}

float4 PS(VS_OUTPUT input):SV_Target
{
	return Sky.Sample(ObjSamplerState,input.PosL);
}