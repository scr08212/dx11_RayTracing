#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace engine
{
    using namespace glm;

    class Object;

    class Hit
    {
    public:
        float d;     // ������ ���ۺ��� �浹 ���������� �Ÿ�
        vec3 point;  // �浹�� ��ġ
        vec3 normal; // �浹�� ��ġ���� ǥ���� ���� ����
        vec2 uv;     // �ؽ��� ��ǥ

        std::shared_ptr<Object> obj; // ���߿� ��ü�� ���� ���� �������� ���� ������
    };
}