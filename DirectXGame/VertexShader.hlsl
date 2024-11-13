struct VS_INPUT
{
	float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 directionToCamera : TEXCOORD1;
};

cbuffer constant: register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
    float time;

    float3 cameraPos;

    float3 color; // albedo color
    float metallic; // metallic strength
    float smoothness; // specular strength
    float flatness; // normal flatness
    float2 tiling; // texture tiling
    float2 offset; // texture offset

    float hasAlbedoMap;
    float hasNormalMap;
    float hasMetallicMap;
    float hasSmoothnessMap;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	//output.position = lerp(input.position, input.position1, (sin(m_angle) + 1.0f) / 2.0f);

    output.position = mul(input.position, world);
    output.directionToCamera = normalize(cameraPos - output.position);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);


    output.texCoord = input.texCoord;
	
	return output;
}