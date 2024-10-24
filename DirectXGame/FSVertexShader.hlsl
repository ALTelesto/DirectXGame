struct VS_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	
	//output.position = lerp(input.position, input.position1, (sin(m_angle) + 1.0f) / 2.0f);

    output.position = input.position;
    output.texCoord = input.texCoord;
	
    return output;
}