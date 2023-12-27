#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "../player/player.h"
#include "floor.h"
#include "object.h"

class Room
{
public:
    vector<vec3> vertices;
    mat4 modelMatrix = mat4(1.0f);
    vec3 roomColor = vec3(0.2f, 0.2f, 0.2f);
    // texture

    GLuint shaderProgram;
    Drawable *drawable;
    float depth = 0;
    float height = 0;
    Floor *floor;
    Floor *ceiling;
    vector<Object *> objects;
    Room(){};
    virtual bool isInside(vec3 position) { return false; }

    void draw(GLuint MLocation, GLuint colorLocation = -1)
    {
        mat4 mainRoomM = modelMatrix;
        drawable->bind();
        
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &mainRoomM[0][0]);
        if (colorLocation != -1)
            glUniform3f(colorLocation, roomColor.x, roomColor.y, roomColor.z);

        drawable->draw();

        floor->draw(MLocation, colorLocation);
        if (colorLocation != -1)
            ceiling->draw(MLocation, colorLocation);

        for (int i = 0; i < objects.size(); i++)
            objects[i]->draw(MLocation, colorLocation);
    }
};

class MainRoom : public Room
{
public:
    float radius;

    MainRoom(float height, float radius, int points);
    bool isInside(vec3 position);
};

class SecondaryRoom : public Room
{
public:
    float width;

    SecondaryRoom(float height, float width, float depth);
    bool isInside(vec3 position);
};