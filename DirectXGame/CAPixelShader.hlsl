struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D sceneTexture : register(t0);
SamplerState samplerState : register(s0);

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float m_angle;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float2 texCoordR = input.texCoord + 0.05 * float2(-1.0, 0.0);
    float2 texCoordG = input.texCoord + 0.05 * float2(-1.0, 1.0);
    float2 texCoordB = input.texCoord + 0.05 * float2(1.0, 0.0);
    
    float4 colorR = sceneTexture.Sample(samplerState, texCoordR);
    float4 colorG = sceneTexture.Sample(samplerState, texCoordG);
    float4 colorB = sceneTexture.Sample(samplerState, texCoordB);
    
    return float4(colorR.r, colorG.g, colorB.b, 1.0);
    //return float4(_texture.Sample(_sampler_state, input.texCoord));
    

}