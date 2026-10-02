#pragma once
#include <SDL3/SDL.h>
#include <smath/smath.h>


// * -------------------- *
// |  Window Positioning  |
// * -------------------- *
struct WindowPosition
{
    /*
        THE STRUCTURE

        (offset.x, offset.y)
         |
         *---------*
         |         |
         |         | < (height)
         |         |
         *---------*
              ^
           (width)
    */

    // Constructors
    WindowPosition(float width, float height, float xoffset, float yoffset)
        : dimensions(smath::vec2(width, height)), offset(smath::vec2(xoffset, yoffset)) {}
    WindowPosition(smath::vec2 dimensions, smath::vec2 offset)
        : dimensions(dimensions), offset(offset) {}

    // Getters
    float getWidth() const { return dimensions.x; }
    float getHeight() const { return dimensions.y; }
    float getXOffset() const { return offset.x; }
    float getYOffset() const { return offset.y; }
    float getXCenter() const { return (dimensions.x * 0.5f) + offset.x; }
    float getYCenter() const { return (dimensions.y * 0.5f) + offset.y; }
    smath::vec2 getOffset() const { return offset; }
    smath::vec2 getCenter() const { return (dimensions * 0.5f) + offset; }
    smath::vec2 getDimensions() const { return dimensions; }
    float getAspectRatio() const { return (dimensions.x / dimensions.y); }
    SDL_FRect getRect() const { return { offset.x, offset.y, dimensions.x, dimensions.y }; }

    SDL_FRect getRectFromBottom(float height) const { return { offset.x, offset.y + dimensions.y - height, dimensions.x, height }; }
    SDL_FRect getRectFromTop(float height) const { return { offset.x, offset.y, dimensions.x, height }; }

    // Setters
    void setDimensions(smath::vec2 dimensions) { this->dimensions = dimensions; }
    void setDimensions(float width, float height) { this->dimensions = smath::vec2(width, height); }
    void setOffset(smath::vec2 offset) { this->offset = offset; }
    void setOffset(float xOffset, float yOffset) { this->offset = smath::vec2(xOffset, yOffset); }
    void setPosition(smath::vec2 dimensions, smath::vec2 offset) { setDimensions(dimensions); setOffset(offset); }
    void setPosition(float width, float height, float xOffset, float yOffset) { setDimensions(width, height); setOffset(xOffset, yOffset); }

    // Member Variables
    smath::vec2 dimensions;
    smath::vec2 offset;
};



// * --------- *
// |  UI Quad  |
// * --------- *
struct UIQuad
{
    /*
        THE STRUCTURE

        (x, y)
         |
         *---------*
         |         |
         |         | < (height: h)
         |         |
         *---------*
              ^
         (width: w)
    */

    // Constructors
    UIQuad() : x(0), y(0), w(0), h(0) {}
    UIQuad(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    UIQuad(smath::vec2 position, smath::vec2 dimensions) : x(position.x), y(position.y), w(dimensions.x), h(dimensions.y) {}
    UIQuad(smath::vec4 quad) : x(quad.x), y(quad.y), w(quad.z), h(quad.w) {}

    // Member Functions (mostly accessors for getting specific data about the quad)

    // Positioning Getters
    //  TL - TC - TR        (0, 0)  -  (0.5, 0)  -  (1, 0)
    //  |          |        |                            |
    //  ML   MC   MR   ->   (0, 0.5)  (0.5, 0.5)  (1, 0.5)
    //  |          |        |                            |
    //  BL - BC - BR        (0, 1)  -  (0.5, 1)  -  (1, 1)

    smath::vec2 TopLeft() const      { return smath::vec2( x,           y           ); } 
    smath::vec2 TopCenter() const    { return smath::vec2( x + (w / 2), y           ); } 
    smath::vec2 TopRight() const     { return smath::vec2( x + w,       y           ); } 
    smath::vec2 MiddleRight() const  { return smath::vec2( x + w,       y + (h / 2) ); } 
    smath::vec2 BottomRight() const  { return smath::vec2( x + w,       y + h       ); } 
    smath::vec2 BottomCenter() const { return smath::vec2( x + (w / 2), y + h       ); } 
    smath::vec2 BottomLeft() const   { return smath::vec2( x,           y + h       ); } 
    smath::vec2 MiddleLeft() const   { return smath::vec2( x,           y + (h / 2) ); } 
    smath::vec2 Center() const       { return smath::vec2( x + (w / 2), y + (h / 2) ); } 

    float getCenterX() const { return (x + (w / 2)); }
    float getCenterY() const { return (y + (h / 2)); }

    // Other Value Getters
    SDL_FRect getSDL_FRect() const { return { x, y, w, h }; }
    smath::vec2 getPosition() const { return smath::vec2(x, y); }
    smath::vec2 getSize() const { return smath::vec2(w, h); }

    // Operator overloads
    UIQuad operator+(const UIQuad& aQuad) const
    {
        return UIQuad(x + aQuad.x, y + aQuad.y, w + aQuad.w, h + aQuad.h);
    }
    UIQuad& operator+=(const UIQuad& aQuad)
    {
        this->x += aQuad.x; 
        this->y += aQuad.y; 
        this->w += aQuad.w; 
        this->h += aQuad.h; 
        return *this;
    }

    UIQuad operator-(const UIQuad& aQuad) const
    {
        return UIQuad(x - aQuad.x, y - aQuad.y, w - aQuad.w, h - aQuad.h);
    }
    UIQuad& operator-=(const UIQuad& aQuad)
    {
        this->x -= aQuad.x;
        this->y -= aQuad.y;
        this->w -= aQuad.w;
        this->h -= aQuad.h;
        return *this;
    }

    // Member variables
    float x;
    float y;
    float w;
    float h;
};



// * -------------------- *
// |  Interactable Stuff  |
// * -------------------- *

// Functions for checking collision
inline bool checkUICollision(smath::vec2 pos, SDL_FRect rect)
{
    //std::cout << "Checking for collision between pos " << pos << " and rect (" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ')' << std::endl;
    bool xCollision = (pos.x > rect.x) && (pos.x < rect.x + rect.w);
    bool yCollision = (pos.y > rect.y) && (pos.y < rect.y + rect.h);
    return (xCollision && yCollision);
}
inline bool checkUICollisionEllipse(smath::vec2 pos, smath::vec2 ellipsePos, smath::vec2 ellipseRadii)
{
    float xCollision = pow(ellipsePos.x - pos.x, 2) / (ellipseRadii.x * ellipseRadii.x);
    float yCollision = pow(ellipsePos.y - pos.y, 2) / (ellipseRadii.y * ellipseRadii.y);
    return (xCollision + yCollision) <= 1.0f;
}

// Abstract Parent Class
class Interactable
{
public:
    // Constructor
    Interactable(smath::vec2 position) : position(position) {}
    Interactable() : position({ 0.0f, 0.0f }) {}

    // Getters
    smath::vec2 getPosition() const { return position; }

    // Setters
    void setPosition(smath::vec2 position) { this->position = position; }

    // Functions
    virtual bool checkCollision(smath::vec2 position) = 0;

protected:
    smath::vec2 position;
};

// Child Class (for Quads)
class QuadInteractable : public Interactable
{
public:
    // Constructors
    QuadInteractable(smath::vec2 position, smath::vec2 size) { setDimensions(position, size); }
    QuadInteractable(SDL_FRect rect) { setDimensions(rect); }

    // Getters
    smath::vec2 getPosition() const { return Interactable::getPosition(); }
    smath::vec2 getSize() const { return size; }
    SDL_FRect getRect() const { return { (float)position.x, (float)position.y, (float)size.x, (float)size.y }; }

    // Setters
    void setDimensions(smath::vec2 position, smath::vec2 size) { Interactable::position = position; this->size = size; }
    void setDimensions(SDL_FRect rect) {
        setDimensions(
            { rect.x, rect.y },
            { rect.w, rect.h });
    }

    // Functions
    bool checkCollision(smath::vec2 position) override { return checkUICollision(position, getRect()); }

private:
    smath::vec2 size;
};

// Child Class (for Ellipses)
class EllipseInteractable : public Interactable
{
public:
    // Constructor
    EllipseInteractable(smath::vec2 position, smath::vec2 radii) : Interactable(position), radii(radii) {}
    EllipseInteractable(smath::vec2 position, float radius) : Interactable(position), radii({ radius, radius }) {}
    EllipseInteractable(smath::vec2 position, float radiusX, float radiusY) : Interactable(position), radii({ radiusX, radiusY }) {}

    // Getters
    smath::vec2 getPosition() const { return Interactable::getPosition(); }
    smath::vec2 getRadii() const { return radii; }

    // Setters
    void setXRadius(float radius) { this->radii.x = radius; }
    void setYRadius(float radius) { this->radii.y = radius; }
    void setRadius(float radius) { this->radii = { radius, radius }; }
    void setRadii(smath::vec2 radii) { this->radii = radii; }

    // Functions
    bool checkCollision(smath::vec2 position) override { return checkUICollisionEllipse(position, Interactable::position, radii); }

private:
    smath::vec2 radii;
};