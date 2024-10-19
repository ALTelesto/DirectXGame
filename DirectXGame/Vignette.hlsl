cbuffer ConstantBuffer : register(b0)
{
    float vignetteRadius;  // How far the vignette reaches (0.0 to 1.0)
    float vignetteStrength; // How strong the vignette effect is (0.0 to 1.0)
};

Texture2D sceneTexture : register(t0);
SamplerState sceneSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float4 color = sceneTexture.Sample(sceneSampler, input.texcoord);

    float2 screenCenter = float2(0.5, 0.5);
    float dist = distance(input.texcoord, screenCenter);

    float vignette = smoothstep(vignetteRadius, vignetteRadius - vignetteStrength, dist);

    color.rgb *= vignette;

    return color;
}