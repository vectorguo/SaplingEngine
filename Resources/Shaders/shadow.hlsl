#include "lighting.hlsl"

cbuffer CBufferObjectCommon : register(b0)
{
	float4x4 SAPLING_MATRIX_M;			//局部坐标到世界坐标的变换矩阵
};

cbuffer CBufferObjectSpecial : register(b1)
{
	float4 _BaseColor;
	float3 _Fresnel;
	float _Metallic;
	float _Roughness;
	float _Placeholder1;
	float _Placeholder2;
	float _Placeholder3;
};

cbuffer CBufferPassCommon : register(b2)
{
	float4x4 SAPLING_MATRIX_V;			//局部坐标到世界坐标的变换矩阵
	float4x4 SAPLING_MATRIX_VP;			//世界坐标到投影坐标的变换矩阵
};

struct VertexIn
{
	float3 PositionOS	: POSITION;
};

struct VertexOut
{
	float4 PositionCS  	: SV_POSITION;
};

VertexOut Vert(VertexIn input)
{
	VertexOut output;
	
	float4 worldPosition = mul(float4(input.PositionOS, 1.0f), SAPLING_MATRIX_M);
	output.PositionCS = mul(worldPosition, SAPLING_MATRIX_VP);
    
    return output;
}

void Frag(VertexOut input)
{

}