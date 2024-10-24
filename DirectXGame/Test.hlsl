struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;  // Texture coordinates from the fullscreen quad
};

float4 psmain(VS_OUTPUT input) : SV_Target
{
    // Output a solid color (e.g., red)
    return float4(1.0f, 0.0f, 0.0f, 1.0f);  // Solid red
}