#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "paintings.h"
#include "../player/player.h"

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius)
{
    vector<Painting *> paintings;

    vec3 colors[] = {
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(0.5, 0.5, 0),
        vec3(0, 0.5, 0.5),
        vec3(0.5, 0, 0.5),
    };

    float angle;

    for (int i = 0; i < n; i++)
    {
        vector<vec3> vertices = {
            vec3(-width / 2, -height / 2, 0),
            vec3(-width / 2, height / 2, 0),
            vec3(width / 2, -height / 2, 0),

            vec3(width / 2, -height / 2, 0),
            vec3(-width / 2, height / 2, 0),
            vec3(width / 2, height / 2, 0),
        };

        angle = 2 * PI * i / n - PI / 2;

        vec3 color = colors[i];
        Painting *painting = new Painting(height, width, color, vertices);
        painting->mainRoomModelMatrix = rotate(mat4(1.0f), angle, vec3(0, 1, 0)) * translate(mat4(1.0f), vec3(0, y_pos, -room_radius * 0.95));
        painting->modelMatrix = painting->mainRoomModelMatrix;

        for (int i = 0; i < 8; i++)
        {
            painting->boundingBox[i] = vec3(painting->modelMatrix * vec4(painting->boundingBox[i], 1.0f));
        }
        paintings.push_back(painting);
    }

    return paintings;
}

#include <iostream>
bool Painting::checkCollision(Player *player)
{
    if (player->boundingBox[0].y < boundingBox[0].y)
    {
        return false;
    }
    if (player->boundingBox[4].y > boundingBox[2].y)
    {
        return false;
    }
    if (abs(player->boundingBox[0].x - boundingBox[0].x) + abs(player->boundingBox[2].x - boundingBox[1].x) + abs(player->boundingBox[0].x - boundingBox[2].x) > abs(boundingBox[0].x - boundingBox[1].x))
    {
        return false;
    }
    if (abs(player->boundingBox[0].z - boundingBox[0].z) + abs(player->boundingBox[1].z - boundingBox[1].z) + abs(player->boundingBox[0].z - boundingBox[1].z) > abs(boundingBox[0].z - boundingBox[1].z))
    {
        return false;
    }
    return true;
}

void Painting::draw(GLuint MLocation, GLuint colorLocation)
{
    if (!visible)
        return;

    glUniformMatrix4fv(MLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform3f(colorLocation, color.x, color.y, color.z);

    drawable->bind();
    drawable->draw();
}