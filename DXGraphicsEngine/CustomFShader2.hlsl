#include "Common.hlsli"

Texture2D texture0 : register(t0);
TextureCube skybox0 : register(t1);
sampler sampler0;

float4 main(FInR input) : SV_TARGET
{
    float4 sampled = texture0.Sample(sampler0, input.uv);
    float4 reflectedSampled = skybox0.Sample(sampler0, input.uvw);
    float4 combined = (input.colour * sampled * (reflectiveness - 1.0)) + (1.0 - reflectedSampled * reflectiveness);
    
    //i have tried to make a fresnel shader but dont know if ive done it exactly, but it looks kinda right
    float4 fresnel = reflectiveness + (1 - reflectiveness) * pow(abs(1.0f - dot(input.normal, input.colour)), 5.0);
    float4 result = lerp(sampled, fresnel, reflectedSampled);
    
    clip(sampled.a < 0.1f ? -1 : 1); //dont render transparency
    return saturate(result);
}