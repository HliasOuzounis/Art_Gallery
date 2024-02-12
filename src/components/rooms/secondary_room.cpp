#include "secondary_room.h"
#include "wall/wall.h"
#include <common/texture.h>

SecondaryRoom::SecondaryRoom(float height, float width, float depth) : width(width)
{
    this->height = height;
    this->depth = depth;

    addWalls(width, height, depth);
    addFloor();
    addCeiling();

    this->light = new Light(vec3(0, height - Light::light_displacement, 0), vec4(1.0, 1.0, 1.0, 1.0), 2.0f, 10.0f);

    addLightBulb();
}

void SecondaryRoom::addWalls(float width, float height, float depth)
{
    Wall *wall1 = new Wall(width, height);
    wall1->translateObject(vec3(0, 0, -depth / 2));
    this->roomObjects.push_back(wall1);

    Wall *wall2 = new Wall(width, height);
    wall2->rotateObject(vec3(0, 1, 0), M_PI);
    wall2->translateObject(vec3(0, 0, depth / 2));
    this->roomObjects.push_back(wall2);
    //*/

    Wall *wall3 = new Wall(depth / 2, height);
    wall3->rotateObject(vec3(0, 1, 0), M_PI / 2);
    wall3->translateObject(vec3(-width / 2, 0, -depth / 4));
    this->roomObjects.push_back(wall3);

    Wall *wall4 = new Wall(depth / 2, height);
    wall4->rotateObject(vec3(0, 1, 0), -M_PI / 2);
    wall4->translateObject(vec3(width / 2, 0, -depth / 4));
    this->roomObjects.push_back(wall4);

    Wall *wall5 = new Wall(depth / 2, height);
    wall5->rotateObject(vec3(0, 1, 0), M_PI / 2);
    wall5->translateObject(vec3(-width / 2, 0, depth / 4));
    this->roomObjects.push_back(wall5);

    Wall *wall6 = new Wall(depth / 2, height);
    wall6->rotateObject(vec3(0, 1, 0), -M_PI / 2);
    wall6->translateObject(vec3(width / 2, 0, depth / 4));
    this->roomObjects.push_back(wall6);
    //*/
    static const Texture wallTexture = {
        "src/assets/textures/walls/blue_walls_diffuse.png",
        "src/assets/textures/walls/blue_walls_specular.png"};
    // static const GLuint normalMap = loadSOIL("src/assets/textures/brick/brickwall_normal.png");
    // static const GLuint wallT = loadSOIL("src/assets/textures/brick/brickwall.png");

    static const GLuint normalMap = loadSOIL("src/assets/textures/brick/bricks2_normal.png");
    static const GLuint wallT = loadSOIL("src/assets/textures/brick/bricks2.png");
    static const GLuint dispMap = loadSOIL("src/assets/textures/brick/bricks2_disp.png");
    

    for (auto wall : roomObjects)
    {
        wall->addTexture(wallTexture, false, false);
        wall->addDiffuseTexture(wallT);
        wall->addNormalTexture(normalMap);
        wall->addDisplacementTexture(dispMap);
    }
}

void SecondaryRoom::addFloor()
{
    Floor *floor = new Floor(width / 2, depth / 2, 0, false);

    static const Texture floorTexture = {
        "src/assets/textures/floor/damaged_wood_diffuse.png",
        "src/assets/textures/floor/damaged_wood_specular.png"};

    floor->addTexture(floorTexture, false, false);

    roomObjects.push_back(floor);
}

void SecondaryRoom::addCeiling()
{
    Floor *ceiling = new Floor(width / 2, depth / 2, height, true);

    ceiling->material = {
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        vec4(0.1, 0.1, 0.1, 1.0),
        1.0};

    roomObjects.push_back(ceiling);
}

bool SecondaryRoom::isInside(vec3 position)
{
    return position.x > -width / 2 + 0.15 and position.x < width / 2 - 0.15 and
           position.y >= 0 and position.y < height and
           position.z > -depth / 2 + 0.15 and position.z < depth / 2 - 0.15;
}