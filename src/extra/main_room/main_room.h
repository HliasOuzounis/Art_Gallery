#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "../player/player.h"

class Room
{
public:
    vector<vec3> vertices;
    mat4 modelMatrix = mat4(1.0f);
    vec3 roomColor = vec3(0.0f, 0.0f, 0.0f);
    // texture
    Drawable *drawable;

    virtual bool isInside(vec3 position) = 0;
    virtual void boundNextPosition(Player *player) = 0;

    void draw(GLuint MVPLocation, GLuint colorLocation, mat4 viewMatrix, mat4 projectionMatrix)
    {
        mat4 mainRoomMVP = projectionMatrix * viewMatrix * modelMatrix;
        drawable->bind();
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mainRoomMVP[0][0]);
        glUniform3f(colorLocation, roomColor.x, roomColor.y, roomColor.z);
        drawable->draw();
    }
};

class MainRoom : public Room
{
public:
    float height;
    float radius;

    MainRoom(float height, float radius, int points);
    bool isInside(vec3 position);
    void boundNextPosition(Player *player);
};