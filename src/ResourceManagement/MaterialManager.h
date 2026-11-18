#ifndef MATERIAL_MANAGER
#pragma once

#include <unordered_map>
#include <cstdlib>
#include "../Material/Material.h"

class MaterialManager
{
public:
    // Consgtructor & Deconstructor
    MaterialManager() {}
    ~MaterialManager() { cleanup(); }

    // Data management functions
    bool init(const std::string& projectRoot);
    void cleanup();

    // Material loading & saving
    void loadMaterialsFromFile();
    void saveMaterialsToFile();
    Material* createMaterialFromFilename(const std::string& filename);

    // Materials map functions
    bool addMaterial(Material* newMat);
    void removeMaterial(uint16_t matKey);
    Material* getMaterial(uint16_t matKey);
    std::unordered_map<uint16_t, Material*>* getMaterialMap() { return &mpMaterialsMap; }

    // Filepath Getters
    std::string getMaterialFilepath(const std::string& materialFilename) const { return mProjectRootPath + MATERIALS_FILEPATH + materialFilename; }
    std::string getShadercodeFilepath(const std::string& shadercodeFilename) const { return mProjectRootPath + SHADERS_FILEPATH + shadercodeFilename; }
    std::string getMaterialsListFilepath() const { return mProjectRootPath + MATERIALS_FILEPATH + "materialsList.txt"; }

    // Misc. Getters
    SDL_GPUGraphicsPipeline* getBasicPipeline() { return mpBasicPipeline; }

private:
    std::string mProjectRootPath = "";
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
    float checker = (floor(input.TexCoord.x * 5.0) + floor(input.TexCoord.y * 5.0)) % 2.0;
    float3 col = float3(lerp(0.6, 0.8, checker), 0.0, 1.0);
    return float4(col, 1.0);
}
)";

    const std::string MATERIALS_FILEPATH = "Materials/";
    const std::string SHADERS_FILEPATH = "Shaders/";
};

#endif // !MATERIAL_MANAGER
