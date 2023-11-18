#include <glm/glm.hpp>

using namespace glm;

class Player
{
public:
    vec3 position;
    vec3 velocity;
    
    Player()
    {
        position = vec3(0, 0, 0);
        velocity = vec3(0, 0, 0);
    }

    void move(float deltaTime);
};