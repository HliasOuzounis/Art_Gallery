#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "paintings.h"
#include "player/player.h"

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius)
{
    vector<Painting *> paintings;

    Material materials[] = {
        // green plastic
        {
            vec4(0.0, 0.0, 0.0, 1.0),
            vec4(0.1, 0.35, 0.1, 1.0),
            vec4(0.45, 0.55, 0.45, 1.0),
            0.25 * 128,
        },
        // red plastic
        {
            vec4(0.0, 0.0, 0.0, 1.0),
            vec4(0.5, 0.0, 0.0, 1.0),
            vec4(0.7, 0.6, 0.6, 1.0),
            0.25 * 128,
        },
        // yellow plastic
        {
            vec4(0.0, 0.0, 0.0, 1.0),
            vec4(0.5, 0.5, 0.0, 1.0),
            vec4(0.6, 0.6, 0.5, 1.0),
            0.25 * 128,
        },
        // cyan plastic
        {
            vec4(0.0, 0.0, 0.0, 1.0),
            vec4(0.0, 0.1, 0.06, 1.0),
            vec4(0.0, 0.50980392, 0.50980392, 1.0),
            0.25 * 128,
        },
        // white plastic
        {
            vec4(0.0, 0.0, 0.0, 1.0),
            vec4(0.55, 0.55, 0.55, 1.0),
            vec4(0.70, 0.70, 0.70, 1.0),
            0.25 * 128,
        },
    };

    float angle;

    Drawable *frame = new Drawable("src/objects/frame.obj");

    for (int i = 0; i < n; i++)
    {
        vector<vec3> vertices = {
            vec3(width / 2, -height / 2, 0),
            vec3(-width / 2, height / 2, 0),
            vec3(-width / 2, -height / 2, 0),

            vec3(width / 2, height / 2, 0),
            vec3(-width / 2, height / 2, 0),
            vec3(width / 2, -height / 2, 0),
        };

        angle = 2 * PI * i / n - PI / 2;

        Painting *painting = new Painting(height, width, materials[i], vertices);
        painting->mainRoomModelMatrix = rotate(mat4(1.0f), angle, vec3(0, 1, 0)) * translate(mat4(1.0f), vec3(0, y_pos, -room_radius * 0.95));
        painting->modelMatrix = painting->mainRoomModelMatrix;

        Material gold = {
            vec4(0.24725, 0.1995, 0.0745, 1.0),
            vec4(0.75164, 0.60648, 0.22648, 1.0),
            vec4(0.628281, 0.555802, 0.366065, 1.0),
            0.4 * 128,
        };
        painting->frame = new Object(frame, gold);
        painting->update_frame_model_matrix();

        for (int i = 0; i < 8; i++)
        {
            painting->boundingBox[i] = vec3(painting->modelMatrix * vec4(painting->boundingBox[i], 1.0f));
        }
        paintings.push_back(painting);
    }

    return paintings;
}

void Painting::update_frame_model_matrix()
{
    this->frame->modelMatrix = this->modelMatrix *
                               translate(mat4(), vec3(1.75, 0, 0)) *
                               rotate(mat4(), (float)PI / 2, vec3(1, 0, 0)) *
                               scale(mat4(), vec3(3.5));
}

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