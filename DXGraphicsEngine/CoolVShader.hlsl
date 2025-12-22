#include "Common.hlsli"
#include "Lighting.hlsli"

struct VOutR
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float3 uvw : TEXCOORD1;
    float4 colour : COLOUR;
};

VOutR main(VIn input)
{
    VOutR output;
    output.position = mul(wvp, float4(sin(frac(dot(input.position.xy, float2(12.9898, 78.233)))) * input.position.x, sin(frac(dot(input.position.yz, float2(78.233, 12.9898)))) * input.position.y, input.position.z, 1));
    output.uv = input.uv;
    output.uvw = CalculateReflectionUVW(world, float4(input.normal, 1), input.position, cameraPosition);
    output.colour = float4(CalculateAllLighting(ambientLightColour.yzx, directionalLights, pointLights, float4(input.normal, 1), input.position), 1);
    return output;
}