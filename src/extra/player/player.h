#include <glm/glm.hpp>

using namespace glm;

class Player
{
public:
    const float height = 1.5f;
    vec3 prevPosition = vec3(0, height, 0);
    vec3 position = vec3(0, height, 0);

    const float speed = 0.2f;
    const float maxSpeed = 5.0f;
    vec3 velocity = vec3(0, 0, 0);

    bool isJumping = true;

    Player(){};

    void move(GLFWwindow *window, float deltaTime, float horizontalAngle);
    void updatePosition(float horizontalAngle, float &deltaTime);
};