#include "Common.hlsli"

Texture2D texture0 : register(t0);
TextureCube skybox0 : register(t1);
sampler sampler0;

float4 main(FInR input) : SV_TARGET
{
    float4 sampled = texture0.Sample(sampler0, input.uv);
    float4 reflectedSampled = skybox0.Sample(sampler0, input.uvw);
    float4 combined = (input.colour * sampled * (reflectiveness - 1.0)) + (1.0 - reflectedSampled * reflectiveness);
    
    //initially tried to make fresnel shader but this looks more interesting
    float4 fresnel = pow(abs(1.0f - dot(input.normal, input.colour)), 5.0);
    float4 result = lerp(sampled, reflectedSampled, fresnel);
    
    clip(sampled.a < 0.1f ? -1 : 1); //dont render transparency
    return saturate(result);
}