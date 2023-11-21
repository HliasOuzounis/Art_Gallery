#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "main_room.h"

MainRoom::MainRoom(float height, float radius, int points) : height(height), radius(radius)
{
    vec3 center_down = vec3(0, 0, 0);
    vec3 center_up = vec3(0, height, 0);

    for (int i = 0; i < points; i++)
    {
        float angle = i * 2 * PI / points;
        float next_angle = (i + 1) * 2 * PI / points;

        vec3 point1 = vec3(radius * cos(angle), 0, radius * sin(angle)),
             point2 = vec3(radius * cos(next_angle), 0, radius * sin(next_angle)),
             point3 = vec3(radius * cos(angle), height, radius * sin(angle)),
             point4 = vec3(radius * cos(next_angle), height, radius * sin(next_angle));

        vertices.push_back(point1);
        vertices.push_back(center_down);
        vertices.push_back(point2);

        vertices.push_back(point4);
        vertices.push_back(center_up);
        vertices.push_back(point3);

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point1);

        vertices.push_back(point2);
        vertices.push_back(point4);
        vertices.push_back(point3);
    }

    this->drawable = new Drawable(vertices);
};

bool MainRoom::isInside(vec3 position)
{
    float distance = pow(position.x, 2) + pow(position.z, 2);
    return 0 <= position.y && position.y <= height && distance <= radius * radius;
};

void MainRoom::boundNextPosition(Player *player)
{
    vec3 position = player->position;
    vec3 prevPosition = player->prevPosition;

    // new(x, y, z) = position * t + prevPosition * (1 - t)
    // also new(x)^2 + new(z)^2 = radius^2
    // so we can solve for t
    float a = position.x + position.z + prevPosition.x * prevPosition.x + prevPosition.z * prevPosition.z 
              - 2 * (position.x * prevPosition.x + position.z * prevPosition.z);
    float b = 2 * (prevPosition.x * (prevPosition.x - 2 * position.x) + prevPosition.z * (prevPosition.z - 2 * position.z));
    float c = prevPosition.x * prevPosition.x + prevPosition.z * prevPosition.z - radius * radius;

    float t = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    if (t > 1 or t < 0){
        float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
    }
    player->position = position * t + prevPosition * (1 - t);
    player->position -= player->playerRadius * normalize(player->position - vec3(0, player->position.y, 0)); 
    player->velocity = vec3(0, player->velocity.y, 0);
};