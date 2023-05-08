#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace engine
{
    using namespace glm;

    class Ray
    {
    public:
        vec3 start;
        vec3 dir;
    };
}