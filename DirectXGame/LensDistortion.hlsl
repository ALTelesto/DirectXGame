struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D sceneTexture : register(t0);
SamplerState samplerState : register(s0);

cbuffer constant_distortion
{
    float distortionStrength;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float2 screenCenter = float2(0.5f, 0.5f);

    float2 coordToCenter = input.texCoord - screenCenter;

    float dist = length(coordToCenter);

    float distortionFactor = 1.0 + distortionStrength * dist * dist;

    float2 distortedTexCoord = screenCenter + coordToCenter * distortionFactor;

    float4 color = sceneTexture.Sample(samplerState, distortedTexCoord);

    return color;
}