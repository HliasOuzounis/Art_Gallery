#include <GL/glew.h> // needed to stop include errors
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

bool Player::collisionWithPainting(Painting *painting)
{
    vec4 bottomLeft = painting->modelMatrix * vec4(-1, -1, 0, 1);
    vec4 bottomRight = painting->modelMatrix * vec4(1, -1, 0, 1);
    float h = painting->height;

    float upperY = bottomLeft.y + h,
          lowerY = bottomLeft.y;

    // check if player is inside the paintings y bounds
    if (position.y <= lowerY || position.y + height >= upperY)
        return false;

    // check if player intersects the paintings plane
    vec3 normal = painting->drawable->normals[0];
    normal = vec3(painting->modelMatrix * vec4(normal, 0));
    float d = -dot(normal, vec3(bottomLeft));

    auto areOppositeSides = [&](vec3 a, vec3 b)
    {
        float dist1 = dot(a, normal) + d;
        float dist2 = dot(b, normal) + d;
        return (dist1 * dist2) < 0;
    };
    bool anyOppositeSides = false;
    for (int i = 0; i < boundingBox.size(); i++)
    {
        for (int j = i + 1; j < boundingBox.size(); j++)
        {
            vec3 pos1 = boundingBox[i] + position;
            vec3 pos2 = boundingBox[j] + position;

            if (areOppositeSides(pos1, pos2))
            {
                anyOppositeSides = true;
                break;
            }
        }
        if (anyOppositeSides)
            break;
    }
    if (!anyOppositeSides)
        return false;
    
    // check if he's inside the paintings x,z bounds
    vec3 normal2 = vec3(rotate(mat4(1.0f), radians(90.0f), vec3(0, 1, 0)) * vec4(normal, 0.0));
    float d2 = -dot(normal2, vec3(bottomLeft));
    vec3 normal3 = vec3(rotate(mat4(1.0f), radians(-90.0f), vec3(0, 1, 0)) * vec4(normal, 0.0));
    float d3 = -dot(normal3, vec3(bottomRight));

    return (dot(position - vec3(4) * normal2 * playerRadius, normal2) + d2 > 0) 
        && (dot(position - vec3(4) * normal3 * playerRadius, normal3) + d3 > 0);
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
