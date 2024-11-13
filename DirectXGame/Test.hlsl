struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 directionToCamera : TEXCOORD1;
};

float4 psmain(PS_INPUT input) : SV_Target
{
    // Output a solid color (e.g., red)
    return float4(1.0f, 0.0f, 0.0f, 1.0f);  // Solid red
}