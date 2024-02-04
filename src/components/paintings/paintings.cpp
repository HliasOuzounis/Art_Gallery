#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

#include <math.h>

using namespace std;
using namespace glm;

#include "paintings.h"
#include "player/player.h"

Painting::Painting()
{
    vector<vec3> vertices = {
        vec3(-1, -1, 0),
        vec3(1, -1, 0),
        vec3(1, 1, 0),
        vec3(-1, -1, 0),
        vec3(1, 1, 0),
        vec3(-1, 1, 0),
    };
    vector<vec2> uvs = {
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(0, 0),
        vec2(1, 1),
        vec2(0, 1),
    };
    vector<vec3> normals = {
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
        vec3(0, 0, 1),
    };
    this->drawable = new Drawable(vertices, uvs, normals);
    addFrame();
    updateFrameModelMatrix();
}

Painting::Painting(float height, float width, float yPos, float mainRoomRadius, float angle) : Painting()
{
    this->scaleObject(vec3(width / 2, height / 2, 1));
    this->translateObject(vec3(0, yPos, 0));
    secondaryRoomModelMatrix = this->modelMatrix;

    this->translateObject(vec3(0, 0, -mainRoomRadius * 0.95));
    this->rotateObject(vec3(0, 1, 0), angle);

    mainRoomModelMatrix = this->modelMatrix;
    updateFrameModelMatrix();
}

void Painting::addFrame()
{
    const Material gold = {
        vec4(0.24725f, 0.1995f, 0.0745f, 1.0f),
        vec4(0.75164f, 0.60648f, 0.22648f, 1.0f),
        vec4(0.628281f, 0.555802f, 0.366065f, 1.0f),
        51.2f};

    Drawable *frame = new Drawable("src/assets/models/frame.obj");

    Object *frameObject = new Object(frame, gold);
    subObjects.push_back(frameObject);
}

void Painting::updateFrameModelMatrix()
{
    subObjects[0]->modelMatrix = this->modelMatrix *
                                 glm::scale(mat4(), vec3(1.5, 1.2, 2)) *
                                 translate(mat4(), vec3(0.53, 0, 0)) *
                                 rotate(mat4(), (float)M_PI / 2, vec3(1, 0, 0));
}