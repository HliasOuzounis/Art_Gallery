#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

#include <common/model.h>
#include <common/texture.h>

using namespace glm;
using namespace std;

#include "object/object.h"

class Wall : public Object
{
public:
    Wall();
    Wall(float width, float height);
};