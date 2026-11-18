// Shadercode for Material: TestMaterial

// Input Struct contains:
// float3 input.Position, world position of the object
// float2 input.TexCoord, texture coordinate of the object

// Available Uniforms:
// float3 _Float3_Uniform;

// Main Function
float4 mainColor(Input input)
{
    return float4(_Float3_Uniform, 1.0);
};
