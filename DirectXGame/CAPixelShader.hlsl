struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

Texture2D texture : register(t0);
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
    float2 texCoord;
    texCoord = input.position.xy / input.position.w;
    texCoord = (texCoord * 0.5f) +0.5f;
    
    float2 texCoordR = texCoord + 0.05f * float2(-1.0, 0.0);
    float2 texCoordG = texCoord;
    float2 texCoordB = texCoord + 0.05f * float2(1.0, 0.0);
    
    float4 colorR = texture.Sample(samplerState, texCoordR);
    float4 colorG = texture.Sample(samplerState, texCoordG);
    float4 colorB = texture.Sample(samplerState, texCoordB);
    
    return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f), 1.0f);
}