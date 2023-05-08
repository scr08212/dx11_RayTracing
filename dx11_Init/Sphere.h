#pragma once
#include "Object.h"

namespace engine
{
    using namespace glm;

    class Sphere : public Object
    {
    public:
        vec3 center;
        float radius;

        Sphere(const vec3& center, const float radius, const vec3& color = vec3(1.0f));
        Hit CheckRayCollision(Ray& ray);
    };
}
