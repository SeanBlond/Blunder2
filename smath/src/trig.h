#ifndef TRIG
#pragma once

namespace smath
{
    constexpr float PI = 3.141593;
    constexpr float TAU = PI * 2.0f;
    constexpr float DEG2RAD = (PI / 180.0f);
    constexpr float RAD2DEG = (180.0f / PI);
    inline float Radians(float degrees)
    {
        return degrees * DEG2RAD;
    }
    inline float Degrees(float radians)
    {
        return radians * RAD2DEG;
    }
}

#endif // !TRIG
