struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D _texture : register(t0);
SamplerState _sampler_state : register(s0);

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
    
    float4 colorR = _texture.Sample(_sampler_state, texCoordR);
    float4 colorG = _texture.Sample(_sampler_state, texCoordG);
    float4 colorB = _texture.Sample(_sampler_state, texCoordB);
    
    return float4(1, 0, 0, 1.0);
    //return float4(_texture.Sample(_sampler_state, input.texCoord));
    

}