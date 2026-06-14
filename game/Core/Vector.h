#pragma once

#include "../RW/RenderWare.h"
#include "Vector2D.h"
#include <cmath>

class CMatrix;

#pragma pack(push, 1)
class CVector : public RwV3d {
public:
    inline CVector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    inline CVector(float X, float Y, float Z) { x = X; y = Y; z = Z; }
    inline CVector(const RwV3d& rwVec) { x = rwVec.x; y = rwVec.y; z = rwVec.z; }

    float Magnitude() const { return std::sqrt(x * x + y * y + z * z); }
    float SquaredMagnitude() const { return x * x + y * y + z * z; }

    void operator+=(const CVector& right) { x += right.x; y += right.y; z += right.z; }
    void operator-=(const CVector& right) { x -= right.x; y -= right.y; z -= right.z; }
    void operator*=(float multiplier) { x *= multiplier; y *= multiplier; z *= multiplier; }
    void operator/=(float divisor) { x /= divisor; y /= divisor; z /= divisor; }

    CVector Cross(const CVector& other) const {
        return CVector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float Dot(const CVector& o) const {
        return x * o.x + y * o.y + z * o.z;
    }

    static auto ZAxisVector() { return CVector(0.f, 0.f, 1.f); }
};
#pragma pack(pop)

static_assert(sizeof(CVector) == 12, "Invalid CVector size");

inline CVector operator-(const CVector& vecOne, const CVector& vecTwo) {
    return CVector(vecOne.x - vecTwo.x, vecOne.y - vecTwo.y, vecOne.z - vecTwo.z);
}

inline CVector operator+(const CVector& vecOne, const CVector& vecTwo) {
    return CVector(vecOne.x + vecTwo.x, vecOne.y + vecTwo.y, vecOne.z + vecTwo.z);
}

inline CVector operator*(const CVector& vec, float multiplier) {
    return CVector(vec.x * multiplier, vec.y * multiplier, vec.z * multiplier);
}
