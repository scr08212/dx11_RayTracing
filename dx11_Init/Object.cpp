#include "Object.h"

engine::Object::Object(const vec3& color)
    : amb(color), dif(color), spec(color)
{
}
