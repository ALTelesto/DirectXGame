struct PS_INPUT
{
    float4 pos : SV_POSITION;
    //float3 color : COLOR;
    float2 texcoord : TEXCOORD;
    float3 directionToCamera: TEXCOORD1;
    //float3 color1 : COLOR1;
    //float fogFactor : FOG;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
    float3 cameraPos;
    float time;

    float3 color;         // albedo color
    float metallicMul;       // metallic strength
    float smoothnessMul;     // specular strength
    float flatness;       // normal flatness
    float2 tiling;        // texture tiling
    float2 offset;        // texture offset

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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	// Sample textures
	float4 albedo = (hasAlbedoMap > 0.5) ? albedoMap.Sample(samplerState, input.texcoord) * float4(color, 1.0) : float4(color, 1.0);
    float3 normal = (hasNormalMap > 0.5) ? (normalMap.Sample(samplerState, input.texcoord).rgb * 2.0 - 1.0) * (1.0 - flatness) + float3(0.0, 0.0, flatness)
                          : float3(0.0, 0.0, 1.0);
    float metallic = (hasMetallicMap > 0.5) ? metallicMap.Sample(samplerState, input.texcoord).r * metallicMul : metallicMul;
    float smoothness = (hasSmoothnessMap > 0.5) ? smoothnessMap.Sample(samplerState, input.texcoord).r * smoothnessMul : smoothnessMul;

    float3 m_light_direction = normalize(float3(0.f, -1.f, 0.f));

    //AMBIENT LIGHT
    float ka = 0.6;
    float3 ia = float3(1.0, 1.0, 1.0);

    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float kd = 10.0 * metallic;
    float3 id = float3(1.0, 1.0, 1.0);
    float amount_diffuse_light = max(0.1 * smoothness, dot(m_light_direction, normal));
    float3 diffuse_light = (kd + 1) * amount_diffuse_light * id;

	//SPECULAR LIGHT
    float ks = metallic * 2.0 + 0.1;
    float3 is = float3(1.0, 1.0, 1.0);
    float3 reflected_light = reflect(-m_light_direction, normal);
    float shininess = lerp(5.0, 100.0, smoothness);
    float amount_specular_light = pow(max(0.0, dot(reflected_light, input.directionToCamera)), shininess);

    float3 specular_light = ks * amount_specular_light * is;

    float3 final_light = ambient_light + diffuse_light + specular_light;

    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    float4 fogColor = float4(0.83f, 0.58f, 0.895f, 1.f);
    // float4 fogColor = float4(1.f, 1.f, 1.f, 1.f);

	color *= albedo;

    if (any(final_light > float3(0.f, 0.f, 0.f)))
    {
        color *= float4(final_light, 1.f);
    }

    //if (any(input.color < float3(1.f, 1.f, 1.f)))
    //{
    //    color *= float4(input.color, 1.f);
    //}

    // color = float4(lerp(input.color, input.color, (sin(time) + 1.0f) / 2.0f), 1.f);

    // return lerp(color, fogColor, input.fogFactor);
    return color;
}

