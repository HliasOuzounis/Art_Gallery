#pragma once

#include <glm/glm.hpp>
#include <glfw3.h>
#include <vector>

using namespace glm;
using namespace std;

#include "paintings/paintings.h"

class Player
{
public:
    const float height = 2.0f;
    const float playerRadius = 0.2f;
    vec3 prevPosition = vec3(0, 0, 0);
    vec3 position = vec3(0, 0, 0);

    const float speed = 0.2f;
    const float maxSpeed = 5.0f;
    vec3 velocity = vec3(0, 0, 0);

    bool isJumping = false;

    vector<vec3> boundingBox = {
        vec3(-playerRadius, 0, -playerRadius),
        vec3(-playerRadius, 0, playerRadius),
        vec3(playerRadius, 0, -playerRadius),
        vec3(playerRadius, 0, playerRadius),
        vec3(-playerRadius, height, -playerRadius),
        vec3(-playerRadius, height, playerRadius),
        vec3(playerRadius, height, -playerRadius),
        vec3(playerRadius, height, playerRadius),
    };

    Player(){};

    void move(GLFWwindow *window, float deltaTime, float horizontalAngle);
    void updatePosition(float horizontalAngle, float &deltaTime);
    
    bool collisionWithPainting(Painting *painting);
    void updateBoundingBox();
};