#include "Uniforms.h"

void Uniform::setName(std::string name)
{
    // Capping the name to 32 characters
    if (name.size() > 32)
        name = name.substr(0, 32);

    // Converting the name to one fit for a variable name
    name = convertToVariable(name);

    // Updating the names
    mUniformName = name;
}

// Comparison operator definition
bool Uniform::operator==(const Uniform& aUniform) const
{
    // Checking if the names are different
    if (this->mUniformName != aUniform.getUniformName())
        return false;

    // Checking if the types are different
    if (this->mType != aUniform.getType())
        return false;

    // Both valuers are the same, return true
    return true;
}

std::string Uniform::convertToVariable(std::string name)
{
    // If the name is empty, return a temporary variable name
    if (name.empty())
        return "_var";
    
    // Deifning the output variable, and that it starts with a '_', if it doesn't already
    std::string outputName = (name[0] == '_' ? "" : "_");

    // Looping through each character
    for (char ch : name)
    {
        // Checking if the chartacter is valid, and converting it to a '_' if it isn't
        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')
            outputName += ch;
        else
            outputName += '_';
    }

    // Returning the output name
    return outputName;
}

// Uniform output savecode functions
std::string IntUniform::getUniformSaveCode()
{
    std::string outputString = "INT " + getUniformName() + ' ' + std::to_string(mValue);
    return outputString;
}
std::string FloatUniform::getUniformSaveCode()
{
    std::string outputString = "FLOAT1 " + getUniformName() + ' ' + std::to_string(mValue);
    return outputString;
}
std::string Float2Uniform::getUniformSaveCode()
{
    std::string outputString = "FLOAT2 " + getUniformName();
    for (int i = 0; i < 2; i++)
    {
        outputString += ' ' + std::to_string(*((float*)(&mValue) + i));
    }
    return outputString;
}
std::string Float3Uniform::getUniformSaveCode()
{
    std::string outputString = "FLOAT3 " + getUniformName();
    for (int i = 0; i < 3; i++)
    {
        outputString += ' ' + std::to_string(*((float*)(&mValue) + i));
    }
    return outputString;
}
std::string Float4Uniform::getUniformSaveCode()
{
    std::string outputString = "FLOAT4 " + getUniformName();
    for (int i = 0; i < 4; i++)
    {
        outputString += ' ' + std::to_string(*((float*)(&mValue) + i));
    }
    return outputString;
}
std::string ColorUniform::getUniformSaveCode()
{
    std::string outputString = "COLOR " + getUniformName();
    for (int i = 0; i < 4; i++)
    {
        outputString += ' ' + std::to_string(*((float*)(&mValue) + i));
    }
    return outputString;
}