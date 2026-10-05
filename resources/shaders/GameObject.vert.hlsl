// Uniform buffer for the matrix buffer
cbuffer MatrixBuffer : register(b0, space1)
{
    float4x4 windowProjection;
};

// Input Values
struct Input
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

// Output Values
struct Output
{
    float4 Position : SV_Position;
    float2 WorldPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

// Function that does the modifications (if any) and outputting
Output main(Input input)
{
    Output output;
    output.Position = mul(float4(input.Position, 1), windowProjection);
    output.WorldPosition = input.Position.xy;
    output.TexCoord = input.TexCoord;
    return output;
}
