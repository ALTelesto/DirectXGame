struct VS_INPUT
{
    float4 pos : POSITION;
    //float3 color : COLOR;
    float2 texcoord : TEXCOORD;
    //float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    //float3 color : COLOR;
    float2 texcoord : TEXCOORD;
    float3 directionToCamera : TEXCOORD1;
};

cbuffer constant : register(b0)
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

// Texture Resources
Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D smoothnessMap : register(t3);

// Sampler for Textures
SamplerState samplerState : register(s0);


VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // World Space
    output.pos = mul(input.pos, world);
    output.directionToCamera = normalize(cameraPos - output.pos);

	// View Space
    output.pos = mul(output.pos, view);

    // Screen Space
    output.pos = mul(output.pos, proj);
    
    output.texcoord = input.texcoord * tiling + offset;


    return output;
}