#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "player/player.h"
#include "floor/floor.h"
#include "object/object.h"

class Room : public Object
{
public:

    float height;
    float depth;
    
    Room(){};
    virtual bool isInside(vec3 position) { return false; }
    virtual void addFloor(){};
    virtual void addCeiling(){};
};