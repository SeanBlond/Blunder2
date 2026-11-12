#ifndef MATERIAL_MANAGER
#pragma once

#include <unordered_map>
#include <cstdlib>
#include "Material.h"

class MaterialManager
{
public:
    // Consgtructor & Deconstructor
    MaterialManager() {}
    ~MaterialManager() { cleanup(); }

    // Data management functions
    bool init(const std::string& materialsFilepath);
    void cleanup();

    // Material loading & saving
    void loadMaterialsFromFile();
    void saveMaterialsToFile();
    Material* createMaterialFromFilepath(const std::string& filepath);

    // Materials map functions
    void addMaterial(Material* newMat);
    Material* getMaterial(uint16_t matKey);
    std::unordered_map<uint16_t, Material*>* getMaterialMap() { return &mpMaterialsMap; }

    // Misc. Getters
    SDL_GPUGraphicsPipeline* getBasicPipeline() { return mpBasicPipeline; }

private:
    std::string mMaterialsFilepath = "";
    std::unordered_map<uint16_t, Material*> mpMaterialsMap;
    SDL_GPUGraphicsPipeline* mpBasicPipeline = nullptr;

    // Function for creating the basic pipeline
    SDL_GPUGraphicsPipeline* generateBasicPipeline();

    // Function for generating unique keys for newly created materials
    uint16_t generateNewKey();

    // Static string of the vertex shader
    inline static const char* FRAGMENT_SHADER_CODE =
        R"(
struct Input
{
    float4 ScreenPosition : SV_Position;
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

float4 main(Input input) : SV_Target0
{
    // Defining uvs
    float2 uv = input.TexCoord;
    
    // Outputting the texture multiplied by the color
    return float4(uv, 0.0, 1.0);
}
)";
};

#endif // !MATERIAL_MANAGER
