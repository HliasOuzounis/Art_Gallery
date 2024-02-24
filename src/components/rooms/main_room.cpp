#include "main_room.h"
#include "wall/wall.h"


MainRoom::MainRoom(float height, float radius, int points) : radius(radius)
{
    this->height = height;

    addWalls(radius, points, height);
    addFloor();
    addCeiling();

    this->light = new Light(vec3(0, height - Light::light_displacement, 0), vec4(1.0, 1.0, 1.0, 1.0), 2.0f, 10.0f);
    addLightBulb();
}
void MainRoom::addWalls(float radius, int points, float height)
{
    float segWidth = 2 * M_PI * radius / points + 0.1f; // 0.1f to compensate for the gaps between the walls
    vector<vec3> vertices;
    for (int i = 0; i < points; i++)
    {
        float angle = i * 2 * M_PI / points;
        float next_angle = (i + 1) * 2 * M_PI / points;

        Wall *wall = new Wall(segWidth, height);
        wall->translateObject(vec3(0, 0, -radius));
        wall->rotateObject(vec3(0, 1, 0), angle);

        roomObjects.push_back(wall);
    }
};

void MainRoom::addFloor()
{

    Floor *floor = new Floor(radius, radius, 0, false);

    static const Texture floorTexture = {
        "src/assets/textures/floor/wood_floor_diffuse.png",
        "src/assets/textures/floor/wood_floor_specular.png"};

    floor->addTexture(floorTexture, false, false);

    roomObjects.push_back(floor);
}

void MainRoom::addCeiling()
{
    Floor *ceiling = new Floor(radius + 0.2, radius + 0.2, height, true); // 0.2f to compensate for the gaps between the walls

    ceiling->material = {
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        1.0};

    roomObjects.push_back(ceiling);
}

bool MainRoom::isInside(vec3 position)
{
    float distance = pow(position.x, 2) + pow(position.z, 2);
    return 0 <= position.y && position.y <= height && distance <= (radius - 0.2) * (radius - 0.2);
};