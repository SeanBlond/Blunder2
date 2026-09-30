// Uniform buffer that contains the transormation matrix
cbuffer MatrixBuffer : register(b0, space1)
{
    float4x4 windowProjection; // Transformation matrix from CPU
};

// Input Values
struct Input
{
    float3 Position : POSITION0;
    float4 Color : COLOR0;
};

// Output Values
struct Output
{
    float4 Position : SV_Position;
    float4 Color : COLOR0;
};

// Function that does the modifications (if any) and outputting
Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    output.Position = mul(float4(input.Position, 1), windowProjection);
    return output;
}
