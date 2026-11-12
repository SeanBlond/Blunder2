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
    Material(std::string materialName, std::string codeFilepath, std::string materialFilepath) :
        mMaterialName(materialName), mCodeFilepath(codeFilepath), mMaterialFilepath(materialFilepath) {}
    ~Material();

    // Shadercode filepath functions
    void loadShadercodeFile();

    // Uniforms Management functions
    void addUniform(Uniform* uniform);
    void addBlankUniform();
    void removeUniformAt(const int& index);
    void replaceUniformAt(const UniformType& newType, const int& index);
    int getUniformCount() const;
    Uniform* getUniformAtIndex(const int& index);
    void manageUniformNames();

    // Graphics Pipeline functions
    bool generateGraphicsPipeline();
    bool bind(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass);
    bool hasValidPipeline() { return (mpPipeline != nullptr); }
    static std::string getBaseVertexCode() { return VERTEX_SHADER_CODE; }

    // Saving function
    void saveMaterialToFile();

    // Getters
    std::string getName() const { return mMaterialName; }
    std::string* getNameAddress() { return &mMaterialName; }
    std::string getMaterialFilepath() const { return mMaterialFilepath; }

private:
    std::string mMaterialName = "";
    std::string mCodeFilepath = "";
    std::string mMaterialFilepath = "";
    std::vector<Uniform*> mpUniforms;
    SDL_GPUGraphicsPipeline* mpPipeline = nullptr;

    // Private function for crteating the fragment shader
    SDL_GPUShader* createFragmentShader();

    // Static max number of uniforms 
    static const int MAX_UNIFORM_AMOUNT = 32;

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
