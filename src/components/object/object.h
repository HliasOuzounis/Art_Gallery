#pragma once

#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Texture
{
    GLuint diffuse = loadSOIL("src/textures/default.png");
    GLuint specular = loadSOIL("src/textures/default.png");
};

class Object
{
public:
    Drawable *drawable;
    vec3 position = vec3(0, 0, 0);
    vec3 rotation = vec3(0, 0, 0);
    vec3 scale = vec3(1, 1, 1);
    mat4 modelMatrix = mat4(1.0f);
    
    Material material;
    Texture texture;
    bool useTexture = false;

    Object(Drawable *drawable);
    Object(Drawable *drawable, Material material);

    void rotateObject(vec3 axis, float angle);
    void scaleObject(vec3 amount);
    void translateObject(vec3 translation);

    void draw(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation);
    void draw(GLuint modelMatrixLocation);
};