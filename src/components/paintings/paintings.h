#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "player/player.h"
#include "object/object.h"

class Painting : public Object
{
public:
    float height, width;
    mat4 mainRoomModelMatrix;
    mat4 secondaryRoomModelMatrix;

    Painting();
    Painting(float height, float width, float yPos, float mainRoomRadius, float angle);

    void update_frame_model_matrix();
    void render(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation){
        Object::render(modelMatrixLocation, materialLocation, useTextureLocation);
        cout << "painting" << endl;
    }

private:
    void addFrame();
};
