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
    
    // Deifning the output variable, and that it starts with a '_'
    std::string outputName = "_";

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