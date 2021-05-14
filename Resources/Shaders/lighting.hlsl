static const float PI = 3.14159265359;

struct Light
{
    float4  Color;
    float3  Direction;
    float   Placeholder1;
    float3  Position;
    float   Placeholder2;
};

float4 Standard_GammaCorrection(float4 color)
{
    float f = 1.0f / 2.2f;
    color.rgb = color.rgb / (color.rgb + float3(1.0f, 1.0f, 1.0f));
    color.rgb = pow(color.rgb, f);
    return color;
}

//正太分布函数
float Standard_DistributionGGX(float nDotH, float roughness)
{
    float a2 = roughness * roughness;
    float denom = (nDotH * nDotH * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return a2 / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

//几何函数
float Standard_GeometrySchlickGGX(float nDot, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return nDot / (nDot * (1.0 - k) + k);
}

//几何函数
float Standard_GeometrySmith(float nDotL, float nDotV, float roughness)
{
    float ggx1 = Standard_GeometrySchlickGGX(nDotL, roughness);
    float ggx2 = Standard_GeometrySchlickGGX(nDotV, roughness);
    return ggx1 * ggx2;
}

//菲涅尔方程
float3 Standard_FresnelSchlick(float cosTheta, float3 f0)
{
    float v = 1.0 - cosTheta;
    return f0 + (1.0 - f0) * v * v * v * v * v;
}

//标准PBR
float3 Standard_Pbr(float3 lightColor, float3 lightDir, float3 normal, float3 toEye, float3 diffuseColor, float3 fresnel, float metallic, float roughness)
{
    float3 halfDir = normalize(toEye + lightDir);
    float nDotH = max(dot(normal, halfDir), 0.0);
    float nDotL = max(dot(normal, lightDir), 0.0);
    float nDotV = max(dot(normal, toEye), 0.0);

    //电解质的基础反射率
	fresnel = lerp(fresnel, diffuseColor.rgb, metallic);

    //计算Cook-Torrance参数
	float  NDF = Standard_DistributionGGX(nDotH, roughness);
    float  G   = Standard_GeometrySmith(nDotL, nDotV, roughness);
    float3 F  = Standard_FresnelSchlick(clamp(dot(halfDir, toEye), 0.0, 1.0), fresnel);

    //计算Cook-Torrance BRDF
	float3 nominator   = NDF * G * F; 
    float  denominator = 4 * nDotV * nDotL;
    float3 specular    = nominator / max(denominator, 0.001);

    float3 kS = F;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0 - metallic;

	float3 color = (diffuseColor + specular) * lightColor * (nDotL * 0.5f + 0.5f);
    return color;
}

float4 Standard_DirectionalLight(Light light, float shadowFactor, float3 normal, float3 toEye, float4 diffuseColor, float3 fresnel, float metallic, float roughness)
{
    float3 pbrColor = Standard_Pbr(light.Color.rgb, -light.Direction, normal, toEye, diffuseColor.rgb, fresnel, metallic, roughness);
    return float4(pbrColor * shadowFactor, diffuseColor.a);
}