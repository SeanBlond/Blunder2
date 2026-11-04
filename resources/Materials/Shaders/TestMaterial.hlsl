// Shadercode for Material: TestMaterial

// Input Struct contains:
// float3 input.Position, world position of the object
// float2 input.TexCoord, texture coordinate of the object

// Available Uniforms:
// int _TestInt;
// float _TestFloat;
// float2 _TestFloat2;

// Main Function
float4 mainColor(Input input) 
{
    float3 outputColor = clamp(input.Position, 0, 1);
    return float4(outputColor, 1.0);
};
