#pragma once

#include "Ray.h"
#include "Hit.h"
#include "Light.h"
#include "Object.h"

#include <vector>

namespace engine
{
    using namespace glm;

    class Raytracer
    {
    public:
        int width, height;
        Light light;
        std::vector<std::shared_ptr<Object>> objects;

        Raytracer(const int& width, const int& height);
        Hit FindClosestCollision(Ray& ray);
        vec3 traceRay(Ray& ray, const int recurseLevel);
        void Render(std::vector<glm::vec4>& pixels);
        vec3 TransformScreenToWorld(vec2 posScreen);
    };
}