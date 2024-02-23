#pragma once

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

#include "object/object.h"

class Painting : public Object
{
public:
    int id;
    float height, width;
    GLuint diffuseTexture, normalTexture;
    mat4 mainRoomModelMatrix;
    mat4 secondaryRoomModelMatrix;

    Painting();
    Painting(float height, float width, float yPos, float mainRoomRadius, float angle);

    void updateFrameModelMatrix();

private:
    void addFrame();
};
