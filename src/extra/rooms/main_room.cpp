#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "main_room.h"

MainRoom::MainRoom(float height, float radius, int points) : height(height), radius(radius)
{
    vec3 center_down = vec3(0, 0, 0);
    vec3 center_up = vec3(0, height, 0);

    vector<vec3> normals;

    for (int i = 0; i < points; i++)
    {
        float angle = i * 2 * PI / points;
        float next_angle = (i + 1) * 2 * PI / points;

        vec3 point1 = vec3(radius * cos(angle), 0, radius * sin(angle)),
             point2 = vec3(radius * cos(next_angle), 0, radius * sin(next_angle)),
             point3 = vec3(radius * cos(angle), height, radius * sin(angle)),
             point4 = vec3(radius * cos(next_angle), height, radius * sin(next_angle));
        
        vec3 normal_1 = -normalize(cross(point2 - point1, point3 - point1));
        vec3 normal_2 = normalize(cross(point2 - point4, point3 - point4));
        vec3 normal_3 = normalize(cross(point1 - point2, center_down - point2));
        vec3 normal_4 = normalize(cross(point1 - point2, center_up - point2));

        vertices.push_back(point1);
        vertices.push_back(center_down);
        vertices.push_back(point2);
        normals.push_back(normal_3);
        normals.push_back(normal_3);
        normals.push_back(normal_3);
        
        vertices.push_back(point4);
        vertices.push_back(center_up);
        vertices.push_back(point3);
        normals.push_back(normal_4);
        normals.push_back(normal_4);
        normals.push_back(normal_4);

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point1);
        normals.push_back(normal_1);
        normals.push_back(normal_1);
        normals.push_back(normal_1);

        vertices.push_back(point2);
        vertices.push_back(point4);
        vertices.push_back(point3);
        normals.push_back(normal_2);
        normals.push_back(normal_2);
        normals.push_back(normal_2);
    }

    this->drawable = new Drawable(vertices, VEC_VEC2_DEFAUTL_VALUE, normals);
};

bool MainRoom::isInside(vec3 position)
{
    float distance = pow(position.x, 2) + pow(position.z, 2);
    return 0 <= position.y && position.y <= height && distance <= (radius - 0.2) * (radius - 0.2);
};

SecondaryRoom::SecondaryRoom(float height, float width) : height(height), width(width)
{
    vec3 point1 = vec3(-width / 2, 0, 0),
         point2 = vec3(width / 2, 0, 0),
         point3 = vec3(-width / 2, height, 0),
         point4 = vec3(width / 2, height, 0),
         point5 = vec3(-width / 2, 0, width),
         point6 = vec3(width / 2, 0, width),
         point7 = vec3(-width / 2, height, width),
         point8 = vec3(width / 2, height, width);

    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point3);

    vertices.push_back(point2);
    vertices.push_back(point3);
    vertices.push_back(point4);

    vertices.push_back(point5);
    vertices.push_back(point6);
    vertices.push_back(point7);

    vertices.push_back(point6);
    vertices.push_back(point7);
    vertices.push_back(point8);

    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point5);

    vertices.push_back(point2);
    vertices.push_back(point5);
    vertices.push_back(point6);

    vertices.push_back(point3);
    vertices.push_back(point4);
    vertices.push_back(point7);

    vertices.push_back(point4);
    vertices.push_back(point7);
    vertices.push_back(point8);

    vertices.push_back(point1);
    vertices.push_back(point3);
    vertices.push_back(point5);

    vertices.push_back(point3);
    vertices.push_back(point5);
    vertices.push_back(point7);

    vertices.push_back(point2);
    vertices.push_back(point4);
    vertices.push_back(point6);

    vertices.push_back(point4);
    vertices.push_back(point6);
    vertices.push_back(point8);

    this->drawable = new Drawable(vertices);
}

bool SecondaryRoom::isInside(vec3 position){
    return position.x > -width/2 and position.x < width/2 and position.y >= 0 and position.y < height and position.z > 0 and position.z < width;
}
