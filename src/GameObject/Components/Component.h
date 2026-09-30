#ifndef COMPONENT
#pragma once

enum ComponentType
{
    COMPONENT_TRANSFORM,
    COMPONENT_MATERIAL,
    COMPONENT_SCRIPT,
    COMPONENT_COLLIDER,
    COMPONENT_RIGIDBODY
};

class Component
{
public:
    // Constructor
    Component(ComponentType type) : mType(type) {}

    // Getters
    ComponentType getType() const { return mType; }

private:
    // Should not be changed after construction
    ComponentType mType;
};

#endif // !COMPONENT
