#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

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

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }

    // handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    }
    
    // projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        position + direction,
        up
    );

    // up and down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }

    //tilt sideways left and right
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        tilt += deltaTime * tiltSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        tilt -= deltaTime * tiltSpeed;
    }

    mat4 tiltMatrix = rotate(mat4(), -tilt, direction);
    viewMatrix = viewMatrix * tiltMatrix;

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}