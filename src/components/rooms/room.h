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

class Room
{
public:
    float height;
    float depth;

    Light *light;
    vector<Object *> roomObjects;

    Room(){};
    virtual bool isInside(vec3 position) { return false; }
    virtual void addFloor(){};
    virtual void addCeiling(){};
    virtual void populateRoom(){};

    void render(GLuint shaderProgram, GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation[3]);
    void render(GLuint depthShader, GLuint modelMatrixLocation);
    void render(GLuint modelMatrixLocation);

protected:
    void addLightBulb();
};