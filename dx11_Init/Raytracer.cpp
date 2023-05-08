#pragma once

#include "Raytracer.h"
#include "Ray.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"
#include "Sphere.h"

#include <vector>

engine::Raytracer::Raytracer(const int& width, const int& height)
    : width(width), height(height)
{
	auto sphere1 = std::make_shared<Sphere>(vec3(0.0f, -0.1f, 1.5f), 1.0f);

	sphere1->amb = vec3(0.2f);
	sphere1->dif = vec3(0.0f, 0.0f, 1.0f);
	sphere1->spec = vec3(0.0f);
	sphere1->alpha = 50.0f;
	sphere1->reflection = 0.0f;
	sphere1->transparency = 1.0f;

	objects.push_back(sphere1);

	//Cubemap

	// front, behind, left, right, top, bottom
	std::shared_ptr<Texture> cubemapTextures[6]{
		std::make_shared<Texture>("Images/Cubemap/front.png"), // + Z
		std::make_shared<Texture>("Images/Cubemap/behind.png"), // - Z
		std::make_shared<Texture>("Images/Cubemap/right.png"), // + X
		std::make_shared<Texture>("Images/Cubemap/left.png"), // - X
		std::make_shared<Texture>("Images/Cubemap/top.png"), // + Y
		std::make_shared<Texture>("Images/Cubemap/bottom.png"), // - Y
	};
	std::shared_ptr<Square> cubemap[6]{
		std::make_shared<Square>(vec3(-10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, 10.0f), vec3(10.0f, -10.0f, 10.0f), vec3(-10.0f, -10.0f, 10.0f), //positive z
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),
		std::make_shared<Square>(vec3(-10.0f, 10.0f, -10.0f), vec3(10.0f, 10.0f, -10.0f), vec3(10.0f, -10.0f, -10.0f), vec3(-10.0f, -10.0f, -10.0f), //negative z
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),

		std::make_shared<Square>(vec3(10.0f, -10.0f, 10.0f), vec3(10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, -10.0f), vec3(10.0f, -10.0f, -10.0f), //positive x
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),
		std::make_shared<Square>(vec3(-10.0f, -10.0f, 10.0f), vec3(-10.0f, 10.0f, 10.0f), vec3(-10.0f, 10.0f, -10.0f), vec3(-10.0f, -10.0f, -10.0f), //negative x
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),

		std::make_shared<Square>(vec3(-10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, -10.0f), vec3(-10.0f, 10.0f, -10.0f), //positive y
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),
		std::make_shared<Square>(vec3(-10.0f, -10.0f, 10.0f), vec3(10.0f, -10.0f, 10.0f), vec3(10.0f, -10.0f, -10.0f), vec3(-10.0f, -10.0f, -10.0f), //negative y
		vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)),
	};

	for (int i = 0; i < 6; i++)
	{
		cubemap[i]->amb = vec3(1.0f);
		cubemap[i]->dif = vec3(0.0f);
		cubemap[i]->spec = vec3(0.0f);
		cubemap[i]->alpha = 10.0f;
		cubemap[i]->reflection = 0.0f;
		cubemap[i]->ambTexture = cubemapTextures[i];
		cubemap[i]->difTexture = cubemapTextures[i];
		objects.push_back(cubemap[i]);
	}

	light = Light{ {0.0f, 0.3f, -0.5f} }; // 화면 뒷쪽
}

engine::Hit engine::Raytracer::FindClosestCollision(Ray& ray)
{
	float closestD = 1000.0; // inf
	Hit closestHit = Hit{ -1.0, dvec3(0.0), dvec3(0.0) };

	for (int l = 0; l < objects.size(); l++)
	{
		auto hit = objects[l]->CheckRayCollision(ray);

		if (hit.d >= 0.0f)
		{
			if (hit.d < closestD)
			{
				closestD = hit.d;
				closestHit = hit;
				closestHit.obj = objects[l];

				closestHit.uv = hit.uv;
			}
		}
	}

	return closestHit;
}

glm::vec3 engine::Raytracer::traceRay(Ray& ray, const int recurseLevel)
{
	if (recurseLevel < 0)
		return vec3(0.0f);

	// Render first hit
	const auto hit = FindClosestCollision(ray);

	if (hit.d >= 0.0f)
	{
		glm::vec3 color(0.0f);

		// Diffuse
		const vec3 dirToLight = glm::normalize(light.pos - hit.point);

		glm::vec3 phongColor(0.0f);

		const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

		// Specular
		const vec3 reflectDir = hit.normal * 2.0f * dot(dirToLight, hit.normal) - dirToLight;
		const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

		if (hit.obj->ambTexture)
		{
			phongColor += hit.obj->amb * hit.obj->ambTexture->SampleLinear(hit.uv);
		}
		else
		{
			phongColor += hit.obj->amb;
		}

		if (hit.obj->difTexture)
		{
			phongColor += diff * hit.obj->dif * hit.obj->difTexture->SampleLinear(hit.uv);
		}
		else
		{
			phongColor += diff * hit.obj->dif;
		}

		phongColor += hit.obj->spec * specular;

		color += phongColor * (1.0f - hit.obj->reflection - hit.obj->transparency);

		if (hit.obj->reflection)
		{
			const auto reflectedDirection = glm::normalize(2.0f * hit.normal * dot(-ray.dir, hit.normal) + ray.dir);
			Ray reflection_ray{ hit.point + reflectedDirection * 1e-4f, reflectedDirection }; // add a small vector to avoid numerical issue

			color += traceRay(reflection_ray, recurseLevel - 1) * hit.obj->reflection;
		}

		// 참고
		// https://samdriver.xyz/article/refraction-sphere (그림들이 좋아요)
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel (오류있음)
		// https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf (슬라이드가 보기 좋지는 않지만 정확해요)
		if (hit.obj->transparency)
		{
			const float ior = 1.5f; // Index of refraction (유리: 1.5, 물: 1.3)

			float eta; // sinTheta1 / sinTheta2
			vec3 normal;

			if (glm::dot(ray.dir, hit.normal) < 0.0f) // 밖에서 안에서 들어가는 경우 (예: 공기->유리)
			{
				eta = ior;
				normal = hit.normal;
			}
			else // 안에서 밖으로 나가는 경우 (예: 유리->공기)
			{
				eta = 1.0f / ior;
				normal = -hit.normal;
			}

			const float cosTheta1 = dot(-ray.dir, normal);
			const float sinTheta1 = sqrt(1.0f - cosTheta1 * cosTheta1); // cos^2 + sin^2 = 1
			const float sinTheta2 = sinTheta1 / eta;
			const float cosTheta2 = sqrt(1.0f - sinTheta2 * sinTheta2);

			const vec3 m = glm::normalize(dot(normal, -ray.dir) * normal + ray.dir);
			const vec3 a = m * sinTheta2;
			const vec3 b = -normal * cosTheta2;
			const vec3 refractedDirection = glm::normalize(a + b); // transmission
			Ray refractedRay{ hit.point + refractedDirection * 1e-4f,refractedDirection };
			color += traceRay(refractedRay, recurseLevel - 1) * hit.obj->transparency;

			// Fresnel 효과는 생략되었습니다.
		}

		return color;
	}

	return vec3(0.0f);
}

void engine::Raytracer::Render(std::vector<vec4>& pixels)
{
	std::fill(pixels.begin(), pixels.end(), vec4(0.0f, 0.0f, 0.0f, 1.0f));

	const vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));
			Ray pixelRay{ pixelPosWorld, glm::normalize(pixelPosWorld - eyePos) };
			pixels[i + width * j] = vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);
		}
}

glm::vec3 engine::Raytracer::TransformScreenToWorld(vec2 posScreen)
{
	const float xScale = 2.0f / this->width;
	const float yScale = 2.0f / this->height;
	const float aspect = float(this->width) / this->height;

	// 3차원 공간으로 확장 (z좌표는 0.0)
	return vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
}