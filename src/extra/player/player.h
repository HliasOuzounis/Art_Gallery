#include <glm/glm.hpp>
#include <glfw3.h>

using namespace glm;

#ifndef PLAYER_H
#define PLAYER_H
class Player
{
public:
    const float height = 1.5f;
    const float playerRadius = 0.1f;
    vec3 prevPosition = vec3(0, 0, 0);
    vec3 position = vec3(0, 0, 0);

    const float speed = 0.2f;
    const float maxSpeed = 5.0f;
    vec3 velocity = vec3(0, 0, 0);

    bool isJumping = false;

    Player(){};

    void move(GLFWwindow *window, float deltaTime, float horizontalAngle);
    void updatePosition(float horizontalAngle, float &deltaTime);
};
#endif