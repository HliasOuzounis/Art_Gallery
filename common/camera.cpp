#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window)
{
    position = glm::vec3(0, 0, 5);
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

    // Task 5.1: simple camera movement that moves in +-z and +-x axes
    // float camera_depth = 5.0f;
    // Move forward
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //     position.z -= 1 * deltaTime * speed;
    // }
    // // Move backward
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //     position.z += 1 * deltaTime * speed;
    // }
    // // Strafe right
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //     position.x += 1 * deltaTime * speed;        
    // }
    // // Strafe left
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //     position.x -= 1 * deltaTime * speed;
    // }
    
    // // Task 5.2: update view matrix so it always looks at the origin
    // projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // viewMatrix = lookAt(
    //     position, // Camera position, in World Space
    //     position - vec3(0, 0, camera_depth),
    //     vec3(0, 1, 0)  // Head is up
    // );

    // Task 5.3: Compute new horizontal and vertical angles, given windows size
    
    // and cursor position
    horizontalAngle -= (xPos - width / 2) * mouseSpeed;
    verticalAngle -= (yPos - height / 2) * mouseSpeed;

    // Task 5.4: right and up vectors of the camera coordinate system
    // use spherical coordinates
    
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

    // Task 5.5: update camera position using the direction/right vectors
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

    // Task 5.6: handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    }
    
    // Task 5.7: construct projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        position + direction,
        up
    );

    // Task2: Make the camera go up and down with the Q and E keys respectively
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }

    // Task3: Make the camera tilt sideways left and right (like peeking around a (corner) with R and T keys.
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