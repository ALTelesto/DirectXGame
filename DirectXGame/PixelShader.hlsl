struct PS_INPUT
{
	float4 position: SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 directionToCamera : TEXCOORD1;
};

Texture2D _texture : register(t0);
SamplerState _sampler_state : register(s0); 

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float m_angle;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(input.texCoord, 0, 1);
    //return float4(_texture.Sample(_sampler_state, input.texCoord));
    //return float4(1,1,1,1);

}