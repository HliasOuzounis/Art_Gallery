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

    float nearPlane = 0.5f;
    float farPlane = 30.0f;
    float light_displacement = -2.0f;
    vec3 direction;

    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 shadowTransforms[6];

    Drawable *drawable;
    mat4 modelMatrix = mat4(1.0f);

    Light(vec3 position, vec4 color, float intensity, float radius);
    void upload_to_shaders(GLuint shaderProgram);
    void upload_depth_shader(GLuint shaderProgram);
    void update_shadow_transforms();

    void draw(GLuint modelMatrixLocation, GLuint materialLocation[4])
    {
        drawable->bind();
        modelMatrix = translate(mat4(), position) * scale(mat4(1.0f), vec3(0.1f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        
        glUniform4fv(materialLocation[0], 1, &color[0]);
        glUniform4fv(materialLocation[1], 1, &color[0]);
        glUniform4fv(materialLocation[2], 1, &color[0]);
        glUniform1f(materialLocation[3], 1.0f);

        drawable->draw();
    }
};