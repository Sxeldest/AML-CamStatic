#pragma once

#include "../RW/RenderWare.h"
#include <cmath>

class CVector;

class CVector2D : public RwV2d {
public:
    inline CVector2D() { x = 0.0f; y = 0.0f; }
    inline CVector2D(float XY) { x = XY; y = XY; }
    inline CVector2D(float X, float Y) { x = X; y = Y; }
    inline CVector2D(const RwV2d& vec2d)     { x = vec2d.x; y = vec2d.y; }

    float Heading() const {
        return std::atan2(-x, y);
    }

    [[nodiscard]] inline float Magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    inline void operator*=(float multiplier) {
        x *= multiplier;
        y *= multiplier;
    }

    [[nodiscard]] inline float SquaredMagnitude() const {
        return x * x + y * y;
    }

    float Dot(const CVector2D& lhs) const {
        return x * lhs.x + y * lhs.y;
    }
};

inline CVector2D operator*(const CVector2D& vec, float multiplier) {
    return CVector2D(vec.x * multiplier, vec.y * multiplier);
}

inline CVector2D operator*(const CVector2D& vecOne, const CVector2D& vecTwo) {
    return CVector2D(vecOne.x * vecTwo.x, vecOne.y * vecTwo.y);
}

inline CVector2D operator+(const CVector2D& vecOne, const CVector2D& vecTwo) {
    return CVector2D(vecOne.x + vecTwo.x, vecOne.y + vecTwo.y);
}

inline CVector2D operator-(const CVector2D& vecOne, const CVector2D& vecTwo) {
    return CVector2D(vecOne.x - vecTwo.x, vecOne.y - vecTwo.y);
}
