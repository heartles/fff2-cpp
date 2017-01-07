#pragma once

#include <cassert>
#include <glm/glm.hpp>

template<typename T>
inline T
sign(T val)
{
    return T((T(0) < val) - (val < T(0)));
}

struct vec2
{
    union
    {
        struct {float x, y;};
        float _arr[2];
    };

    inline vec2(float xx, float yy)
    {
        x = xx;
        y = yy;
    }

    inline vec2()
    {
        x = 0;
        y = 0;
    }

    inline float & operator [] (int index)
    {
        assert(index < 2);
        return _arr[index];
    }

    inline float Length()
    {
        return std::sqrt(LengthSquared());
    }

    inline float LengthSquared()
    {
        return x * x + y * y;
    }

    inline vec2 Normalize()
    {
        auto len = Length();

        if (len == 0)
            return vec2();
        return vec2{ x / len, y / len };
    }

    inline vec2 operator * (float scalar)
    {
        return vec2{x * scalar, y * scalar};
    }

    inline vec2 operator + (vec2 other)
    {
        return vec2{x + other.x, y + other.y};
    }

    inline vec2 operator += (vec2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

struct vec3
{
    union
    {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float _arr[3];
    };

    inline vec3(float xx, float yy, float zz)
    {
        x = xx;
        y = yy;
        z = zz;
    }

    inline vec3()
    {
        x = 0;
        y = 0;
    }

    inline float & operator [] (int index)
    {
        assert(index < 3);
        return _arr[index];
    }

    inline float Length()
    {
        return std::sqrt(LengthSquared());
    }

    inline float LengthSquared()
    {
        return x * x + y * y + z * z;
    }

    inline vec3 normalize()
    {
        auto len = Length();

        if (len == 0)
            return vec3();
        return vec3{ x / len, y / len, z / len };
    }
};

template <typename T>
inline T
normalize_zero(const T& vector)
{
    if (glm::length(vector) == 0)
        return T();

    return glm::normalize(vector);
}

template <typename T>
inline T
Identity()
{
    T output = {};

    for (int i = 0; i < output.length(); ++i) {
        output[i][i] = 1;
    }

    return output;
}

glm::mat3
Translate(vec2 amount);

glm::mat3
Scale(vec2 amount);

glm::mat3
Rotate(float amount);

/*
inline
template<typename T>
T
sign(T val) {
    return T(T(0) < val) - (val < T(0));
}
*/
