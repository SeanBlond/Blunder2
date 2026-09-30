#ifndef MAT4
#pragma once

#include <algorithm>
#include <initializer_list>
#include "vectors.h"
#include "ivectors.h"

namespace smath
{
    struct mat4
    {
        // Default constructor (identity matrix)
        mat4();
        mat4(std::initializer_list<float> list);

        // Operator Overloads w/ other vector
        mat4 operator*(const mat4& aMat) const;
        mat4& operator*=(const mat4& aMat);

        mat4& operator=(const mat4& aMat);

        // Accessor operators
        float& operator()(int row, int col) { return mData[row * 4 + col]; }
        const float& operator()(int row, int col) const { return mData[row * 4 + col]; }

        // ostream Operator
        friend std::ostream& operator<<(std::ostream& os, const mat4& mat);

        // 4x4 float stored as a simple array of 16 floats
        float mData[16];
    };

    // mat4 transformation generation functions
    smath::mat4 translate(smath::vec3 position);
    smath::mat4 scale(smath::vec3 scalar);

}

#endif