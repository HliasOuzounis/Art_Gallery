#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

using namespace glm;

#include "player.h"

void Player::move(GLFWwindow *window, float deltaTime, float horizontalAngle)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        velocity.y = 5.0f;
        isJumping = true;
        updatePosition(horizontalAngle, deltaTime);
        return;
    }

    if (isJumping)
    {
        velocity.y -= 9.8f * deltaTime;
        if (position.y + velocity.y * deltaTime < 0)
        {
            position.y = 0;
            velocity.y = 0;
            isJumping = false;
        }
    }

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        velocity.z -= speed;
    // Move backward
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        velocity.z += speed;

    else if (!isJumping)
        velocity.z = 0;

    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        velocity.x += speed;
    // Strafe left
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        velocity.x -= speed;

    else if (!isJumping)
        velocity.x = 0;

    velocity = clamp(velocity, -maxSpeed, maxSpeed);

    updatePosition(horizontalAngle, deltaTime);
}

void Player::updatePosition(float horizontalAngle, float &deltaTime)
{
    // Rotate the velocity vector to match the global coordinate system
    mat4 rotation = rotate(mat4(1.0f), horizontalAngle, vec3(0, 1, 0));

    vec3 movement = vec3(rotation * vec4(velocity, 1.0)) * deltaTime;
    prevPosition = position;
    position += movement;
}

void Player::updateBoundingBox()
{
    vec3 movement = position - prevPosition;
    mat4 translation = translate(mat4(1.0f), movement);
    for (int i = 0; i < 8; i++)
    {
        boundingBox[i] = vec3(translation * vec4(boundingBox[i], 1.0f));
    }
}
