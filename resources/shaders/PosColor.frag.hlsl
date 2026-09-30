// Input Struct
struct Input
{
    float4 Position : SV_Position;
    float4 Color: COLOR0;
};

// Main Function
float4 main(Input input) : SV_Target0
{
    // Outputting the color
    return float4(input.Color);
}