#ifndef UNIFORMS
#pragma once

#include <string>
#include <cctype>
#include <smath/smath.h>

// Various data types that can be passed into a material
enum UniformType
{
    UNIFORM_INT,
    UNIFORM_FLOAT,
    UNIFORM_FLOAT2,
    UNIFORM_FLOAT3,
    UNIFORM_FLOAT4,
};

// * ---------------------- *
// |  Uniform Parent Class  |
// * ---------------------- *
class Uniform
{
public:
    // Constructor
    Uniform(std::string name, UniformType type) : mType(type) { setName(name); }

    // Name management
    std::string getUniformName() const { return mUniformName; }
    void setName(std::string name);

    // Type Management
    UniformType getType() const { return mType; }

    // Comparison operator
    bool operator==(const Uniform& aUniform) const;

    // Pure virtual function for data accessing
    virtual void* getData() = 0;
    virtual uint32_t getSize() = 0;

private:
    std::string mUniformName;
    UniformType mType;

    static std::string convertToVariable(std::string name);
};


// * ----------------------- *
// |  Uniform Child Classes  |
// * ----------------------- *

// Int Uniform
class IntUniform : public Uniform
{
public:
    IntUniform(std::string name, const int& value) : mValue(value), Uniform(name, UNIFORM_INT) {}

    // Value getter override
    void* getData() override { return static_cast<void*>(&mValue); }
    uint32_t getSize() override { return (uint32_t)sizeof(int); }

private:
    int mValue;
};

// Float Uniform
class FloatUniform : public Uniform
{
public:
    FloatUniform(std::string name, const float& value) : mValue(value), Uniform(name, UNIFORM_FLOAT) {}

    // Value getter override
    void* getData() override { return static_cast<void*>(&mValue); }
    uint32_t getSize() override { return (uint32_t)sizeof(float); }


private:
    float mValue;
};

// Float2 Uniform
class Float2Uniform : public Uniform
{
public:
    Float2Uniform(std::string name, const smath::vec2& value) : mValue(value), Uniform(name, UNIFORM_FLOAT2) {}

    // Value getter override
    void* getData() override { return static_cast<void*>(&mValue); }
    uint32_t getSize() override { return (uint32_t)sizeof(smath::vec2); }

private:
    smath::vec2 mValue;
};

// Float3 Uniform
class Float3Uniform : public Uniform
{
public:
    Float3Uniform(std::string name, const smath::vec3& value) : mValue(value), Uniform(name, UNIFORM_FLOAT3) {}

    // Value getter override
    void* getData() override { return static_cast<void*>(&mValue); }
    uint32_t getSize() override { return (uint32_t)sizeof(smath::vec3); }

private:
    smath::vec3 mValue;
};

// Float4 Uniform
class Float4Uniform : public Uniform
{
public:
    Float4Uniform(std::string name, const smath::vec4& value) : mValue(value), Uniform(name, UNIFORM_FLOAT4) {}

    // Value getter override
    void* getData() override { return static_cast<void*>(&mValue); }
    uint32_t getSize() override { return (uint32_t)sizeof(smath::vec4); }

private:
    smath::vec4 mValue;
};


// Function for converting UniformTypes to strings
inline std::string UniformTypeToString(UniformType type)
{
    std::string output = "";

    switch (type)
    {
    case UNIFORM_INT:
        output = "int";
        break;
    case UNIFORM_FLOAT:
        output = "float";
        break;
    case UNIFORM_FLOAT2:
        output = "float2";
        break;
    case UNIFORM_FLOAT3:
        output = "float3";
        break;
    case UNIFORM_FLOAT4:
        output = "float4";
        break;
    }

    return output;
}


#endif // !UNIFORMS
