#include "Common.hlsli"
#include "Lighting.hlsli"

VOutR main(VIn input)
{
    VOutR output;
    output.position = mul(wvp, float4(input.position, 1));
    output.uv = input.uv;
    output.uvw = CalculateReflectionUVW(world, float4(input.position, 1), input.normal, cameraPosition);
    output.normal = input.normal;
    output.colour = float4(CalculateAllLighting(ambientLightColour.xyz, directionalLights, pointLights, float4(input.position, 1), input.normal), 1);
    return output;
}