#include "lighting.hlsl"

Texture2D _BaseMap : register(t0);

SamplerState SamplerPointWrap        : register(s0);
SamplerState SamplerPointClamp       : register(s1);
SamplerState SamplerLinearWrap       : register(s2);
SamplerState SamplerLinearClamp      : register(s3);

cbuffer CBufferObjectCommon : register(b0)
{
	float4x4 SAPLING_MATRIX_M;			//局部坐标到世界坐标的变换矩阵
};

cbuffer CBufferObjectSpecial : register(b1)
{
	float4 _BaseColor;
	float3 _Fresnel;
	float _Roughness;
};

cbuffer CBufferPassCommon : register(b2)
{
	float4x4 SAPLING_MATRIX_V;			//局部坐标到世界坐标的变换矩阵
	float4x4 SAPLING_MATRIX_VP;			//世界坐标到投影坐标的变换矩阵

	float4 AmbientLightColor;			//环境光

	float3 WorldSpaceCameraPosition;	//世界坐标下的相机位置

	float Placeholder1;					//占位符

	Light MainLight;					//主光源
};

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
	float3 PositionWS	: POSITION;
	float3 NormalWS		: NORMAL;
    float4 Color 		: COLOR;
	float2 UV0			: TEXCOORD;
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
    
    return output;
}

float4 Frag(VertexOut input) : SV_Target
{
	float4 diffuseTex = _BaseMap.Sample(SamplerLinearWrap, input.UV0) * _BaseColor;
	float3 normal = normalize(input.NormalWS);
	float3 toEye = normalize(WorldSpaceCameraPosition - input.PositionWS);

	//环境光
	float4 ambient = diffuseTex * AmbientLightColor;
	
	//方向光
	float4 color = Standard_DirectionalLight(MainLight, normal, toEye, diffuseTex, _Fresnel, 1 - _Roughness);
    return ambient + color;
}


