#include "floor.h"

#include "object/object.h"

Floor::Floor(float width, float depth)
{
    vec3 point1 = vec3(-width, 0, depth);
    vec3 point2 = vec3(-width, 0, -depth);
    vec3 point3 = vec3(width, 0, depth);
    vec3 point4 = vec3(width, 0, -depth);

    vec3 normal = vec3(0, 1, 0);

    vector<vec3> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;

    vertices.push_back(point2);
    vertices.push_back(point1);
    vertices.push_back(point3);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(1, 1));

    vertices.push_back(point2);
    vertices.push_back(point3);
    vertices.push_back(point4);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));

    for (int i = 0; i < 6; i++)
        normals.push_back(normal);
    this->drawable = new Drawable(vertices, uvs, normals);

    this->material = {
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        1.0f};
}

Floor::Floor(float width, float depth, float height, bool isCeiling) : Floor(width, depth)
{
    if (isCeiling)
    {
        this->rotateObject(vec3(0, 0, 1), 3.1415f);
    }
    this->translateObject(vec3(0, height, 0));
}
