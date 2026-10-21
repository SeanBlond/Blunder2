#include "Material.h"
#include "../graphics/ShaderCross.h"

// Constructor & Deconstructor definitions
Material::Material()
{
    // Adding some random uniforms
    addUniform(new IntUniform("TestInt", 10));
    addUniform(new FloatUniform("TestFloat", 3.1415f));
    addUniform(new Float2Uniform("TestFloat2", smath::vec2(1.0f, 2.0f)));

    // Generate code
    generateGraphicsPipeline();
}
Material::~Material()
{
    // Deallocating all uniforms
    for (Uniform* uniform : mpUniforms)
    {
        delete uniform;
        uniform = nullptr;
    }
    mpUniforms.clear();
}

// Uniforms Management function definitions
void Material::addUniform(Uniform* uniform)
{
    mpUniforms.push_back(uniform);
}
int Material::getUniformCount() const
{
    return mpUniforms.size();
}
Uniform* Material::getUniformAtIndex(const int& index)
{
    // Returning null if index is out of range
    if (index < 0 || index >= mpUniforms.size())
        return nullptr;

    return mpUniforms[index];
}

// Graphics Pipeline function definitions
void Material::generateGraphicsPipeline()
{
    // Defining the stringstream that will contain the shader code
    std::stringstream shaderCodeStream;
    
    // Adding the basic input code
    shaderCodeStream <<
R"(
struct Input
{
    float4 Position : SV_Position;
    float2 WorldPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};
)" << std::endl;

    // If there are any uniforms, add them as a uniform buffer
    if (mpUniforms.size() != 0)
    {
        // Adding the buffer to the stream
        shaderCodeStream << "cbuffer UBO : register(b0, space0) \n{\n";

        // Adding each uniform
        for (Uniform* uniform : mpUniforms)
        {
            shaderCodeStream << '\t' << UniformTypeToString(uniform->getType()) << ' ' << uniform->getPrivateName() << ";\n";
        }

        // Closing the buffer
        shaderCodeStream << "};\n";
    }

    // TODO: Adding in the user-written code here

    // Adding in the propper main function
    shaderCodeStream <<
R"(
float4 mainOutput(Input input) : SV_Target0
{
    float4 shaderOutput = Output(input);
    return shaderOutput;
};
)";

    // Creating a pipeline from the code


}
void Material::bind(SDL_GPURenderPass* renderPass)
{

}