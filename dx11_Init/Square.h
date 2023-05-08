#pragma once

#include "Triangle.h"

namespace engine
{
    class Square : public Object
    {
    public:
        Triangle triangle1, triangle2;

        Square(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec2 uv0 = vec2(0.0f), vec2 uv1 = vec2(0.0f), vec2 uv2 = vec2(0.0f), vec2 uv3 = vec2(0.0f));

        virtual Hit CheckRayCollision(Ray& ray);
    };
}