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

    Drawable *drawable;
    mat4 modelMatrix = mat4(1.0f);

    Light(vec3 position, vec4 color, float intensity, float radius);
    void upload_to_shaders(GLuint shaderProgram);
    mat4 get_light_space_matrix();

    void draw(GLuint MLocation, GLuint colorLocation)
    {
        drawable->bind();
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3f(colorLocation, color.x, color.y, color.z);
        drawable->draw();
    }
};