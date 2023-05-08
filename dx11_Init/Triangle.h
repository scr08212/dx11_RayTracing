#pragma once

#include "Object.h"

namespace engine
{
    using namespace glm;
    class Triangle : public Object
    {
    public:
        vec3 v0, v1, v2;
        vec2 uv0, uv1, uv2; // 뒤에서 텍스춰 좌표계로 사용

    public:
        Triangle();
        Triangle(vec3 v0, vec3 v1, vec3 v2, vec2 uv0 = vec2(0.0f), vec2 uv1 = vec2(0.0f), vec2 uv2 = vec2(0.0f));
        virtual Hit CheckRayCollision(Ray& ray);
        bool IntersectRayTriangle(const vec3& orig, const vec3& dir,
            const vec3& v0, const vec3& v1,
            const vec3& v2, vec3& point, vec3& faceNormal,
            float& t, float& w0, float& w1);
    };
}