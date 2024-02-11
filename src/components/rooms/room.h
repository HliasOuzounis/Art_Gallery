#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "player/player.h"
#include "floor/floor.h"
#include "object/object.h"
#include "light/light.h"

class Room : public Object
{
public:
    using Object::render;

    float height;
    float depth;

    Light *light;

    Room(){};
    virtual bool isInside(vec3 position) { return false; }
    virtual void addFloor(){};
    virtual void addCeiling(){};

    void render(GLuint shaderProgram, GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation[3]);
    void render(GLuint depthShader, GLuint modelMatrixLocation);

protected:
    void addLightBulb();
};