#include "wall.h"

#include "object/object.h"

Wall::Wall()
{
    vec3 point1 = vec3(0, 0, 0);
    vec3 point2 = vec3(0, 1, 0);
    vec3 point3 = vec3(1, 1, 0);
    vec3 point4 = vec3(1, 0, 0);

    vector<vec3> vertices;
    vector<vec2> uvs;

    vertices.push_back(point3);
    vertices.push_back(point2);
    vertices.push_back(point1);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(0, 0));

    vertices.push_back(point3);
    vertices.push_back(point1);
    vertices.push_back(point4);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));

    vec3 normal = vec3(0, 0, 1);
    vector<vec3> normals = {};
    for (int i = 0; i < 6; i++)
        normals.push_back(normal);
    
    this->drawable = new Drawable(vertices, uvs, normals);
    this->material = {
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        1.0f};
    
    this->translateObject(vec3(-0.5, 0, 0));
}

Wall::Wall(float width, float height) : Wall()
{
    this->scaleObject(vec3(width, height, 1));
}