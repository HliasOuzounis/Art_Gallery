#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Light
{
public:
    vec3 position;
    vec4 color;
    float intensity;
    float radius;

    float near_plane = 1.0f;
    float far_plane = 25.0f;
    vec3 direction;

    mat4 projectionMatrix;
    mat4 viewMatrix;

    Light(vec3 position, vec4 color, float intensity, float radius);
    void upload_to_shaders(GLuint shaderProgram);
    mat4 get_light_space_matrix();
};