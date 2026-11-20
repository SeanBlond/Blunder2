// Input Struct
struct Input
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

// Reserving space for texture and sampler
Texture2D ColorTexture : register(t0, space2);
SamplerState ColorSampler : register(s0, space2);

// Main Function
float4 main(Input input) : SV_Target0
{
    // Sampling the texture
    float4 image = ColorTexture.Sample(ColorSampler, input.TexCoord);
    
    // Outputting the texture multiplied by the color
    return image;;
}