struct Light
{
    float4 Color;
    float3 Direction;
    float Placeholder1;
    float3 Position;
    float Placeholder2;
};

//菲涅尔反射
float3 Standard_SchlickFresnel(float3 r0, float3 normal, float3 lightDirection)
{
    float cosIncidentAngle = saturate(dot(normal, -lightDirection));
    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = r0 + (1 - r0) * (f0 * f0 * f0 * f0 * f0);
    return reflectPercent;
}

//标准PBR
float3 Standard_Pbr(float3 lightColor, float3 lightDirection, float3 normal, float3 toEye, float3 diffuseColor, float3 fresnel, float shininess)
{
    const float m = shininess * 256.0f;
    float3 halfVec = normalize(toEye - lightDirection);
    float roughnessFactor = (m + 0.8f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = Standard_SchlickFresnel(fresnel, halfVec, lightDirection);
    float3 specAlbedo = fresnelFactor * roughnessFactor;
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);
    return (diffuseColor + specAlbedo) * lightColor;
}

float4 Standard_DirectionalLight(Light light, float3 normal, float3 toEye, float4 diffuseColor, float3 fresnel, float shininess)
{
    float nDotL = max(dot(-light.Direction, normal), 0.0f);
    float3 pbrColor = Standard_Pbr(light.Color.rgb * nDotL, light.Direction, normal, toEye, diffuseColor.rgb, fresnel, shininess);
    return float4(pbrColor, diffuseColor.a);
}