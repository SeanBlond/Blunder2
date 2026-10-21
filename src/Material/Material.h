#ifndef MATERIAL
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <SDL3/SDL.h>
#include "Uniforms.h"

class Material
{
public:
    // Constructor & Deconstructor
    Material();
    ~Material();

    // Uniforms Management functions
    void addUniform(Uniform* uniform);
    int getUniformCount() const;
    Uniform* getUniformAtIndex(const int& index);

    // Graphics Pipeline functions
    void generateGraphicsPipeline();
    void bind(SDL_GPURenderPass* renderPass);

private:
    std::string mCodeFilepath = "";
    std::vector<Uniform*> mpUniforms;
    SDL_GPUGraphicsPipeline* mpPipeline = nullptr;

    // Private function for converting 
};

#endif // !MATERIAL
