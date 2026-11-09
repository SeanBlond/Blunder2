#ifndef MATERIAL
#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <SDL3/SDL.h>
#include "Uniforms.h"

class Material
{
public:
    // Constructor & Deconstructor
    Material(std::string materialName, std::string codeFilepath);
    ~Material();

    // Shadercode filepath functions
    void loadShadercodeFile();

    // Uniforms Management functions
    void addUniform(Uniform* uniform);
    void replaceUniformAt(UniformType newType, int index);
    int getUniformCount() const;
    Uniform* getUniformAtIndex(const int& index);

    // Graphics Pipeline functions
    bool generateGraphicsPipeline();
    bool bind(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass);
    bool hasValidPipeline() { return (mpPipeline != nullptr); }
    static std::string getBaseVertexCode() { return VERTEX_SHADER_CODE; }

    // Getters
    std::string getName() const { return mMaterialName; }
    std::string* getNameAddress() { return &mMaterialName; }

private:
    std::string mMaterialName = "";
    std::string mCodeFilepath = "";
    std::vector<Uniform*> mpUniforms;
    SDL_GPUGraphicsPipeline* mpPipeline = nullptr;

    // Private function for crteating the fragment shader
    SDL_GPUShader* createFragmentShader();

    // Static string of the vertex shader
    inline static const char* VERTEX_SHADER_CODE =
R"(
cbuffer MatrixBuffer : register(b0, space1)
{
    float4x4 windowProjection;
    float4x4 worldTransformation;
};
struct Input
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};
struct Output
{
    float4 ScreenPosition : SV_Position;
    float3 Position: POSITION0;
    float2 TexCoord : TEXCOORD0;
};
Output main(Input input)
{
    Output output;
    output.ScreenPosition = mul(float4(input.Position, 1), windowProjection);
    output.Position = mul(float4(input.Position, 1), worldTransformation).xyz;
    output.TexCoord = input.TexCoord;
    return output;
}
)";
};

#endif // !MATERIAL
