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
    update_frame_model_matrix();
}

Painting::Painting(float height, float width, float yPos, float mainRoomRadius, float angle) : Painting()
{
    this->scaleObject(vec3(width / 2, height / 2, 1));
    this->translateObject(vec3(0, yPos, 0));
    secondaryRoomModelMatrix = this->modelMatrix;

    this->translateObject(vec3(0, 0, -mainRoomRadius * 0.95));
    this->rotateObject(vec3(0, 1, 0), angle);

    mainRoomModelMatrix = this->modelMatrix;
    update_frame_model_matrix();
}

void Painting::addFrame()
{
    const Material tintedGold = {
        vec4(0.24725, 0.1995, 0.0745, 1),
        vec4(0.75164, 0.60648, 0.22648, 1),
        vec4(0.628281, 0.555802, 0.366065, 1),
        0.4f,
    };
    Drawable *frame = new Drawable("src/models/frame.obj");

    Object *frameObject = new Object(frame, tintedGold);
    subObjects.push_back(frameObject);
}

void Painting::update_frame_model_matrix()
{
    subObjects[0]->modelMatrix = this->modelMatrix *
        glm::scale(mat4(), vec3(1.5, 1.2, 2)) *
        translate(mat4(), vec3(0.5, 0, 0)) *
        rotate(mat4(), (float)M_PI / 2, vec3(1, 0, 0));
}