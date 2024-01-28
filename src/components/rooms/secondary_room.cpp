#include "secondary_room.h"

SecondaryRoom::SecondaryRoom(float height, float width, float depth) : width(width)
{
    this->height = height;
    this->depth = depth;

    vec3 point1 = vec3(-width / 2, 0, -depth / 2),
         point2 = vec3(width / 2, 0, -depth / 2),
         point3 = vec3(-width / 2, height, -depth / 2),
         point4 = vec3(width / 2, height, -depth / 2),
         point5 = vec3(-width / 2, 0, 0),
         point6 = vec3(width / 2, 0, 0),
         point7 = vec3(-width / 2, height, 0),
         point8 = vec3(width / 2, height, 0),
         point9 = vec3(-width / 2, 0, depth / 2),
         point10 = vec3(width / 2, 0, depth / 2),
         point11 = vec3(-width / 2, height, depth / 2),
         point12 = vec3(width / 2, height, depth / 2);

    vec3 normal1 = normalize(cross(point2 - point1, point3 - point1)),
         normal2 = -normalize(cross(point1 - point3, point5 - point3)),
         normal3 = -normalize(cross(point4 - point2, point6 - point2)),
         normal4 = normalize(cross(point5 - point6, point7 - point6));

    vector<vec3> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;

    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point3);
    normals.push_back(normal1);
    normals.push_back(normal1);
    normals.push_back(normal1);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point4);
    vertices.push_back(point3);
    vertices.push_back(point2);
    normals.push_back(normal1);
    normals.push_back(normal1);
    normals.push_back(normal1);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    vertices.push_back(point5);
    vertices.push_back(point1);
    vertices.push_back(point7);
    normals.push_back(normal2);
    normals.push_back(normal2);
    normals.push_back(normal2);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point3);
    vertices.push_back(point7);
    vertices.push_back(point1);
    normals.push_back(normal2);
    normals.push_back(normal2);
    normals.push_back(normal2);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    vertices.push_back(point2);
    vertices.push_back(point6);
    vertices.push_back(point4);
    normals.push_back(normal3);
    normals.push_back(normal3);
    normals.push_back(normal3);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point8);
    vertices.push_back(point4);
    vertices.push_back(point6);
    normals.push_back(normal3);
    normals.push_back(normal3);
    normals.push_back(normal3);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    vertices.push_back(point9);
    vertices.push_back(point5);
    vertices.push_back(point11);
    normals.push_back(normal2);
    normals.push_back(normal2);
    normals.push_back(normal2);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point7);
    vertices.push_back(point11);
    vertices.push_back(point5);
    normals.push_back(normal2);
    normals.push_back(normal2);
    normals.push_back(normal2);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    vertices.push_back(point6);
    vertices.push_back(point10);
    vertices.push_back(point8);
    normals.push_back(normal3);
    normals.push_back(normal3);
    normals.push_back(normal3);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point12);
    vertices.push_back(point8);
    vertices.push_back(point10);
    normals.push_back(normal3);
    normals.push_back(normal3);
    normals.push_back(normal3);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    vertices.push_back(point10);
    vertices.push_back(point9);
    vertices.push_back(point12);
    normals.push_back(normal4);
    normals.push_back(normal4);
    normals.push_back(normal4);
    uvs.push_back(vec2(0, 0));
    uvs.push_back(vec2(1, 0));
    uvs.push_back(vec2(0, 1));

    vertices.push_back(point11);
    vertices.push_back(point12);
    vertices.push_back(point9);
    normals.push_back(normal4);
    normals.push_back(normal4);
    normals.push_back(normal4);
    uvs.push_back(vec2(1, 1));
    uvs.push_back(vec2(0, 1));
    uvs.push_back(vec2(1, 0));

    this->drawable = new Drawable(vertices, uvs, normals);

    this->texture = {
        "src/assets/textures/walls/blue_walls_diffuse.png",
        "src/assets/textures/walls/blue_walls_specular.png"};
    this->useTexture = true;

    this->light = new Light(vec3(0, height - Light::light_displacement, 0), vec4(1.0, 1.0, 1.0, 1.0), 150.0f, 10.0f);

    addFloor();
    addCeiling();
    addLightBulb();
}

void SecondaryRoom::addFloor()
{
    Floor *floor = new Floor(width / 2, depth / 2, 0, false);

    floor->texture = {
        "src/assets/textures/floor/damaged_wood_diffuse.png",
        "src/assets/textures/floor/damaged_wood_specular.png"};

    floor->useTexture = true;

    subObjects.push_back(floor);
}

void SecondaryRoom::addCeiling()
{
    Floor *ceiling = new Floor(width / 2, depth / 2, height, true);

    ceiling->material = {
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        1.0};

    subObjects.push_back(ceiling);
}

bool SecondaryRoom::isInside(vec3 position)
{
    return position.x > -width / 2 + 0.15 and position.x < width / 2 - 0.15 and
           position.y >= 0 and position.y < height and
           position.z > -depth / 2 + 0.15 and position.z < depth / 2 - 0.15;
}