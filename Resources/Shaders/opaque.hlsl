//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer CBufferPerObject : register(b0)
{
	float4x4 SAPLING_MATRIX_M;			//局部坐标到世界坐标的变换矩阵
};

cbuffer CBufferPerPass : register(b1)
{
	float4x4 SAPLING_MATRIX_V;			//局部坐标到世界坐标的变换矩阵
	float4x4 SAPLING_MATRIX_VP;			//世界坐标到投影坐标的变换矩阵

	float3 MAIN_LIGHT_POSITION;
	float3 MAIN_LIGHT_DIRECTION;
};

struct VertexIn
{
	float3 PositionOS	: POSITION;
	float3 NormalOS		: NORMAL;
    float4 Color		: COLOR;
	float2 UV0			: TEXCOORD0;
	float2 UV1			: TEXCOORD1;
};

struct VertexOut
{
	float4 PositionCS  	: SV_POSITION;
	float3 NormalWS		: NORMAL;
    float4 Color 		: COLOR;
};

VertexOut Vert(VertexIn input)
{
	VertexOut output;
	
	output.PositionCS = mul(mul(float4(input.PositionOS, 1.0f), SAPLING_MATRIX_M), SAPLING_MATRIX_VP);
	output.NormalWS = mul(input.NormalOS, (float3x3)SAPLING_MATRIX_M);
    output.Color = input.Color;
    
    return output;
}

float4 Frag(VertexOut input) : SV_Target
{
	float3 normal = normalize(input.NormalWS);
	float nDotL = dot(MAIN_LIGHT_DIRECTION, normal);
	nDotL = nDotL * 0.5 + 0.5;
    return half4(0, 1, 0, 1) * nDotL;
}


