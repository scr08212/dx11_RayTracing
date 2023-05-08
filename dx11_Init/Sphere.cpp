#include "Sphere.h"

engine::Sphere::Sphere(const vec3& center, const float radius, const vec3& color)
    : center(center), radius(radius), Object(color)
{
}

engine::Hit engine::Sphere::CheckRayCollision(Ray& ray)
{
    Hit hit = Hit{ -1.0f, vec3(0.0f), vec3(0.0f) };

    const float b = 2.0f * glm::dot(ray.dir, ray.start - this->center);
    const float c = dot(ray.start - this->center, ray.start - this->center) - this->radius * this->radius;

    const float det = b * b - 4.0f * c;
    if (det >= 0.0f)
    {
        const float d1 = (-b - sqrt(det)) / 2.0f;
        const float d2 = (-b + sqrt(det)) / 2.0f;
        hit.d = glm::min(d1, d2);

        // 물체 안에서 다시 밖으로 나가면서 충돌 가능
        if (hit.d < 0.0f)
            hit.d = glm::max(d1, d2);

        hit.point = ray.start + ray.dir * hit.d;
        hit.normal = glm::normalize(hit.point - this->center);
    }

    return hit;
}