#include "lighting.hlsl"

Texture2D _ShadowMap : register(t0);
Texture2D _BaseMap : register(t1);

SamplerState SamplerPointWrap        : register(s0);
SamplerState SamplerPointClamp       : register(s1);
SamplerState SamplerLinearWrap       : register(s2);
SamplerState SamplerLinearClamp      : register(s3);
SamplerComparisonState SamplerShadow : register(s4);

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

    float4x4 SAPLING_MATRIX_SHADOW;		//世界坐标到阴影坐标的变换矩阵

	float4 AmbientLightColor;			//环境光

	float3 WorldSpaceCameraPosition;	//世界坐标下的相机位置

	float Placeholder1;					//占位符

	Light MainLight;					//主光源
};

//---------------------------------------------------------------------------------------
// PCF for shadow mapping.
//---------------------------------------------------------------------------------------

float CalcShadowFactor(float4 shadowCoord)
{
    // Complete projection by doing division by w.
    shadowCoord.xyz /= shadowCoord.w;

    // Depth in NDC space.
    float depth = shadowCoord.z;

    uint width, height, numMips;
    _ShadowMap.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float)width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
    };

    [unroll]
    for(int i = 0; i < 9; ++i)
    {
        percentLit += _ShadowMap.SampleCmpLevelZero(SamplerShadow, shadowCoord.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}