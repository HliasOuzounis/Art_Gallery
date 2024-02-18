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

    static const Texture wallTexture = {
        "src/assets/textures/walls/blue_walls_diffuse.png",
        "src/assets/textures/walls/blue_walls_specular.png"};
    // static const GLuint normalMap = loadSOIL("src/assets/textures/brick/brickwall_normal.png");
    // static const GLuint wallT = loadSOIL("src/assets/textures/brick/brickwall.png");

    static const GLuint normalMap = loadSOIL("src/assets/textures/brick/bricks2_normal.png");
    static const GLuint wallT = loadSOIL("src/assets/textures/brick/bricks2.png");

    for (auto wall : roomObjects)
    {
        wall->addTexture(wallTexture, false, false);
        // wall->addDiffuseTexture(wallT);
        // wall->addNormalTexture(normalMap);
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

void SecondaryRoom::populateRoom()
{
    static Drawable *wooden_table = new Drawable("src/assets/models/wooden_table.obj");
    static GLuint tableDiffuse = loadSOIL("src/assets/textures/objects/table/table.png");
    static GLuint tableSpecular = loadSOIL("src/assets/textures/objects/table/tableSpecular.png");
    static GLuint tableNormal = loadSOIL("src/assets/textures/objects/table/tableNormal.png");
    Object *tableObject = new Object(wooden_table);
    tableObject->addDiffuseTexture(tableDiffuse);
    tableObject->addSpecularTexture(tableSpecular);
    // table->addNormalTexture(tableNormal);
    tableObject->scaleObject(vec3(1.2, 0.6, 1.2));
    tableObject->rotateObject(vec3(0, 1, 0), - 3 * M_PI / 4 - 0.3);
    tableObject->translateObject(vec3(2, 0, 5));

    roomObjects.push_back(tableObject);

    static Drawable *bed = new Drawable("src/assets/models/bed.obj");
    static GLuint bedDiffuse = loadSOIL("src/assets/textures/objects/bed/bed.png");
    static GLuint bedSpecular = loadSOIL("src/assets/textures/objects/bed/bedSpecular.png");
    static GLuint bedNormal = loadSOIL("src/assets/textures/objects/bed/bedNormal.png");
    Object *bedObject = new Object(bed);
    bedObject->addDiffuseTexture(bedDiffuse);
    bedObject->addSpecularTexture(bedSpecular);
    // bedObject->addNormalTexture(bedNormal);
    bedObject->rotateObject(vec3(0, 1, 0), M_PI);
    bedObject->scaleObject(vec3(3, 3, 3));
    bedObject->translateObject(vec3(-2.5, 0, 4));

    roomObjects.push_back(bedObject);

    static Drawable *chair = new Drawable("src/assets/models/chair.obj");
    static GLuint chairDiffuse = loadSOIL("src/assets/textures/objects/chair/chair.png");
    static GLuint chairSpecular = loadSOIL("src/assets/textures/objects/chair/chairSpecular.png");
    static GLuint chairNormal = loadSOIL("src/assets/textures/objects/chair/chairNormal.png");
    Object *chairObject = new Object(chair);
    chairObject->addDiffuseTexture(chairDiffuse);
    chairObject->addSpecularTexture(chairSpecular);
    // chairObject->addNormalTexture(chairNormal);
    chairObject->rotateObject(vec3(0, 1, 0), -M_PI / 2);
    chairObject->scaleObject(vec3(2, 2, 2));
    chairObject->translateObject(vec3(3, 0, 1));

    roomObjects.push_back(chairObject);

    Object *chairObject2 = new Object(chair);
    chairObject2->addDiffuseTexture(chairDiffuse);
    chairObject2->addSpecularTexture(chairSpecular);
    // chairObject2->addNormalTexture(chairNormal);
    chairObject2->rotateObject(vec3(0, 1, 0), -3 * M_PI / 4 + 0.2);
    chairObject2->scaleObject(vec3(2, 2, 2));
    chairObject2->translateObject(vec3(-0.5, 0, 6));

    roomObjects.push_back(chairObject2);

    static Drawable *window = new Drawable("src/assets/models/window.obj");
    static GLuint windowDiffuse = loadSOIL("src/assets/textures/objects/window/window.png");
    static GLuint windowSpecular = loadSOIL("src/assets/textures/objects/window/windowSpecular.png");
    static GLuint windowNormal = loadSOIL("src/assets/textures/objects/window/windowNormal.png");
    Object *windowObject = new Object(window);
    windowObject->addDiffuseTexture(windowDiffuse);
    windowObject->addSpecularTexture(windowSpecular);
    // windowObject->addNormalTexture(windowNormal);
    windowObject->rotateObject(vec3(1, 0, 0), -M_PI / 2);
    windowObject->scaleObject(vec3(0.3, 0.3, 0.3));
    windowObject->translateObject(vec3(1, this->height / 2 + 0.5, this->depth / 2));

    roomObjects.push_back(windowObject);

    static Drawable *paintingFrame = new Drawable("src/assets/models/paintingFrame.obj");
    static GLuint paintingFrameDiffuse = loadSOIL("src/assets/textures/objects/paintings/paintingFrame.png");
    static GLuint paintingFrameSpecular = loadSOIL("src/assets/textures/objects/paintings/paintingFrameSpecular.png");
    Object *paintingFrameObject = new Object(paintingFrame);
    paintingFrameObject->addDiffuseTexture(paintingFrameDiffuse);
    paintingFrameObject->addSpecularTexture(paintingFrameSpecular);
    paintingFrameObject->scaleObject(vec3(0.04, 0.04, 0.04));
    paintingFrameObject->translateObject(vec3(-7, 1.5, this->depth / 2));

    roomObjects.push_back(paintingFrameObject);

    Object *paintingFrameObject2 = new Object(paintingFrame);
    paintingFrameObject2->addDiffuseTexture(paintingFrameDiffuse);
    paintingFrameObject2->addSpecularTexture(paintingFrameSpecular);
    paintingFrameObject2->scaleObject(vec3(0.04, 0.04, 0.04));
    paintingFrameObject2->translateObject(vec3(-5, 0, 0));
    paintingFrameObject2->rotateObject(vec3(0, 1, 0), M_PI / 2);
    paintingFrameObject2->translateObject(vec3(-this->width / 2, 2, 3));

    roomObjects.push_back(paintingFrameObject2);
}