#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

#include <common/model.h>
#include <common/texture.h>

using namespace glm;
using namespace std;

#include "object/object.h"

class Floor : public Object
{
public:
    Floor(float width, float depth);
    Floor(float width, float depth, float height, bool isCeiling);
};