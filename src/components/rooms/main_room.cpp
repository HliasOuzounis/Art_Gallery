#include "main_room.h"


MainRoom::MainRoom(float height, float radius, int points) : radius(radius)
{
    this->height = height;

    vec3 center_down = vec3(0, 0, 0);
    vec3 center_up = vec3(0, height, 0);

    vector<vec3> vertices;
    for (int i = 0; i < points; i++)
    {
        float angle = i * 2 * M_PI / points;
        float next_angle = (i + 1) * 2 * M_PI / points;

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

    vector<vec3> normals;
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

    this->material = {
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        1.0};

    this->light = new Light(vec3(0, height - Light::light_displacement, 0), vec4(1.0, 1.0, 1.0, 1.0), 300.0f, 10.0f);

    addFloor();
    addCeiling();
    addLightBulb();
};

void MainRoom::addFloor()
{

    Floor *floor = new Floor(radius, radius, 0, false);

    static const Texture floorTexture = {
        "src/assets/textures/floor/wood_floor_diffuse.png",
        "src/assets/textures/floor/wood_floor_specular.png"};

    floor->addTexture(floorTexture, false, false);

    subObjects.push_back(floor);
}

void MainRoom::addCeiling()
{
    Floor *ceiling = new Floor(radius, radius, height, true);

    ceiling->material = {
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        1.0};

    subObjects.push_back(ceiling);
}

bool MainRoom::isInside(vec3 position)
{
    float distance = pow(position.x, 2) + pow(position.z, 2);
    return 0 <= position.y && position.y <= height && distance <= (radius - 0.2) * (radius - 0.2);
};