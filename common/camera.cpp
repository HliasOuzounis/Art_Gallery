#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <algorithm>
#define PI 3.14159265f

using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window)
{
    position = glm::vec3(0, 1.5, 1);
    horizontalAngle = 0.0f;
    verticalAngle = 0.0f;
    FoV = 45.0f;
    speed = 3.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
}

float tilt = 0.0f;
float tiltSpeed = 1.0f;

void Camera::update()
{
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2, height / 2);
    
    // and cursor position
    horizontalAngle -= (xPos - width / 2) * mouseSpeed;
    verticalAngle -= (yPos - height / 2) * mouseSpeed;
    verticalAngle = clamp(verticalAngle, -PI/2, PI/2);

    // right and up vectors of the camera coordinate system, use spherical coordinates
    
    vec3 direction(
        - cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        - cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    vec3 right = vec3(
        -cos(horizontalAngle),
        0,
        sin(horizontalAngle)
    );

    // Up vector
    vec3 up = normalize(cross(direction, right));


    // handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        position -= right * deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        freeform = !freeform;
    }

    // projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        position + direction,
        up
    );


    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}