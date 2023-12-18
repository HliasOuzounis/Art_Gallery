#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>
#include <iostream>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "main_room.h"

MainRoom::MainRoom(float height, float radius, int points) : height(height), radius(radius)
{
    this->floor = new Floor(radius, radius, 0, false);
    this->ceiling = new Floor(radius, radius, height, true);

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

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point1);

        vertices.push_back(point2);
        vertices.push_back(point4);
        vertices.push_back(point3);
    }
    vector<vec3> triangleNormals;
    for (int i = 0; i < vertices.size(); i += 3)
    {
        vec3 v1 = vertices[i];
        vec3 v2 = vertices[i + 1];
        vec3 v3 = vertices[i + 2];
        vec3 normal = cross(v2 - v1, v3 - v1);
        triangleNormals.push_back(normal);
        triangleNormals.push_back(normal);
        triangleNormals.push_back(normal);
    }
    for (int i = 0; i < vertices.size(); i++)
    {
        vec3 vertexNormal = vec3(0.0);
        for (int j = 0; j < vertices.size(); j++)
        {
            if (vertices[i] == vertices[j])
            {
                vertexNormal += triangleNormals[j];
            }
        }
        vertexNormal = normalize(vertexNormal);
        normals.push_back(vertexNormal);
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