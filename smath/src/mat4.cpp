#include "mat4.h"

namespace smath
{
    // * ----------------------- *
    // |  mat4 struct functions  |
    // * ----------------------- *

    // Constructors
    mat4::mat4()
    {
        mData[0] = 1.0f; mData[1] = 0.0f; mData[2] = 0.0f; mData[3] = 0.0f;
        mData[4] = 0.0f; mData[5] = 1.0f; mData[6] = 0.0f; mData[7] = 0.0f;
        mData[8] = 0.0f; mData[9] = 0.0f; mData[10] = 1.0f; mData[11] = 0.0f;
        mData[12] = 0.0f; mData[13] = 0.0f; mData[14] = 0.0f; mData[15] = 1.0f;
    }
    mat4::mat4(std::initializer_list<float> list)
    {
        // Filling up to the initialization list size, or the regular 16 floats
        std::copy_n(list.begin(), std::min(list.size(), size_t(16)), mData);;

        // If list has lessthan 16 float values, fill the rest with zeroes
        std::fill(mData + list.size(), mData + 16, 0.0f);
    }

    // Operator Overloads w/ other vector
    mat4 mat4::operator*(const mat4& aMat) const
    {
        //std::cout << " New Matrix: " << std::endl;
        mat4 tempMat4;
        for (int row = 0; row < 4; ++row) 
        {
            for (int col = 0; col < 4; ++col) 
            {
                tempMat4(row, col) = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    tempMat4(row, col) += (*this)(row, k) * aMat(k, col);
                }
            }
        }
        return tempMat4;
    }
    mat4& mat4::operator*=(const mat4& aMat)
    {
        mat4 tempMat4;
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                tempMat4(row, col) = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    tempMat4(row, col) += (*this)(row, k) * aMat(k, col);
                }
            }
        }

        // Copying the result then returning it
        *this = tempMat4;
        return *this;
    }
    mat4& mat4::operator=(const mat4& aMat)
    {
        // Copying the data over
        std::copy(aMat.mData, aMat.mData + 16, mData);

        // Returning the matrix
        return *this;
    }

    // * ---------------- *
    // |  mat4 functions  |
    // * ---------------- *
    // mat4 transformation generation functions
    smath::mat4 translate(smath::vec3 position)
    {
        return {
            1.0f, 0.0f, 0.0f, position.x,
            0.0f, 1.0f, 0.0f, position.y,
            0.0f, 0.0f, 1.0f, position.z,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }
    smath::mat4 scale(smath::vec3 scalar)
    {
        return {
            scalar.x, 0.0f,     0.0f,     0.0f,
            0.0f,     scalar.y, 0.0f,     0.0f,
            0.0f,     0.0f,     scalar.z, 0.0f,
            0.0f,     0.0f,     0.0f,     1.0f,
        };
    }
    smath::mat4 rotateX(const float& angle)
    {
        return 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos(angle), sin(angle), 0.0f,
            0.0f, -sin(angle), cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    smath::mat4 rotateY(const float& angle)
    {
        return 
        {
            cos(angle), 0.0f, -sin(angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sin(angle), 0.0f, cos(angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    smath::mat4 rotateZ(const float& angle)
    {
        return 
        {
            cos(angle), sin(angle), 0.0f, 0.0f,
            -sin(angle), cos(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    smath::mat4 rotate(smath::vec3 angles)
    {
        smath::mat4 xRotation, yRotation, zRotation;
        xRotation = rotateX(angles.x);
        yRotation = rotateY(angles.y);
        zRotation = rotateZ(angles.z);

        smath::mat4 rotationMatrix = (xRotation * zRotation * yRotation);
        return rotationMatrix;
    }
    smath::mat4 orthographic(float l, float r, float b, float t)
    {
        // Creating the Matrix
        smath::mat4 orthMatrix;
        orthMatrix(0, 0) = 2 / (r - l);
        orthMatrix(0, 1) = 0;
        orthMatrix(0, 2) = 0;
        orthMatrix(0, 3) = 0;

        orthMatrix(1, 0) = 0;
        orthMatrix(1, 1) = 2 / (t - b);
        orthMatrix(1, 2) = 0;
        orthMatrix(1, 3) = 0;

        orthMatrix(2, 0) = 0;
        orthMatrix(2, 1) = 0;
        orthMatrix(2, 2) = 1;
        orthMatrix(2, 3) = 0;

        orthMatrix(3, 0) = -((r + l) / (r - l));
        orthMatrix(3, 1) = -((t + b) / (t - b));
        orthMatrix(3, 2) = 0;
        orthMatrix(3, 3) = 1;

        return orthMatrix;
    }

    smath::mat4 orthographic(float l, float r, float b, float t, float n, float f)
    {
        // Creating the Matrix
        smath::mat4 orthMatrix;
        orthMatrix(0, 0) = 2 / (r - l);
        orthMatrix(0, 1) = 0;
        orthMatrix(0, 2) = 0;
        orthMatrix(0, 3) = 0;

        orthMatrix(1, 0) = 0;
        orthMatrix(1, 1) = 2 / (t - b);
        orthMatrix(1, 2) = 0;
        orthMatrix(1, 3) = 0;

        orthMatrix(2, 0) = 0;
        orthMatrix(2, 1) = 0;
        orthMatrix(2, 2) = 2 / (f - n);
        orthMatrix(2, 3) = 0;

        orthMatrix(3, 0) = -((r + l) / (r - l));
        orthMatrix(3, 1) = -((t + b) / (t - b));
        orthMatrix(3, 2) = -((f + n) / (f - n));
        orthMatrix(3, 3) = 1;

        return orthMatrix;
    }

    // Ostream operator
    std::ostream& operator<<(std::ostream& os, const mat4& mat)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << mat(i, j) << ", ";
            }
            std::cout << std::endl;
        }
        return os;
    }
}