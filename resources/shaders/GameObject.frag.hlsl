// Input Struct
struct Input
{
    float4 Position : SV_Position;
    float2 WorldPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

// Main Function
float4 main(Input input) : SV_Target0
{
    // Defining uvs
    float2 uv = input.TexCoord;
    
    // Outputting the texture multiplied by the color
    return float4(uv, 0.0, 1.0);
}