struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D _texture : register(t0);
SamplerState _sampler_state : register(s0);

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float2 texCoordR = input.texCoord + 0.005 * float2(-1.0, 0.0);
    float2 texCoordG = input.texCoord + 0.005 * float2(-1.0, 1.0);
    float2 texCoordB = input.texCoord + 0.005 * float2(1.0, 0.0);
    
    float4 colorR = _texture.Sample(_sampler_state, texCoordR);
    float4 colorG = _texture.Sample(_sampler_state, texCoordG);
    float4 colorB = _texture.Sample(_sampler_state, texCoordB);
    
    return float4(colorR.r, colorG.g, colorB.b, 1.0);
    //return float4(_texture.Sample(_sampler_state, input.texCoord));
    

}