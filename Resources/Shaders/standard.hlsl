#include "common.hlsl"

struct VertexIn
{
	float3 PositionOS	: POSITION;
	float3 NormalOS		: NORMAL;
    float4 Color		: COLOR;
	float2 UV0			: TEXCOORD;
	float2 UV1			: TEXCOORD1;
};

struct VertexOut
{
	float4 PositionCS  	: SV_POSITION;
	float3 NormalWS		: NORMAL;
    float4 Color 		: COLOR;
	float2 UV0			: TEXCOORD0;
	float3 PositionWS	: TEXCOORD1;
	float4 ShadowCoord	: TEXCOORD2;
};

VertexOut Vert(VertexIn input)
{
	VertexOut output;
	
	float4 worldPosition = mul(float4(input.PositionOS, 1.0f), SAPLING_MATRIX_M);
	output.PositionCS = mul(worldPosition, SAPLING_MATRIX_VP);
	output.PositionWS = worldPosition.xyz;
	output.NormalWS = mul(input.NormalOS, (float3x3)SAPLING_MATRIX_M);
    output.Color = input.Color;
	output.UV0 = input.UV0;

	output.ShadowCoord = mul(worldPosition, SAPLING_MATRIX_SHADOW);
    
    return output;
}

float4 Frag(VertexOut input) : SV_Target
{
	float4 diffuseTex = _BaseMap.Sample(SamplerLinearWrap, input.UV0) * _BaseColor;
	float3 normal = normalize(input.NormalWS);
	float3 toEye = normalize(WorldSpaceCameraPosition - input.PositionWS);

	//阴影系数
	float shadowFactor = CalcShadowFactor(input.ShadowCoord);

	//环境光
	float4 ambient = diffuseTex * AmbientLightColor;
	
	//方向光
	float4 color = Standard_DirectionalLight(MainLight, shadowFactor, normal, toEye, diffuseTex, _Fresnel, _Metallic, _Roughness);
    return ambient + color;
}


