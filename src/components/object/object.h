#pragma once

#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "src/constants.h"

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
    static GLuint defaultTexture;

    GLuint diffuse;
    GLuint specular;
    GLuint normalMap;
    GLuint displacementMap;

    // Constructor with default values
    Texture()
    {
        if (defaultTexture == 0)
        {
            defaultTexture = loadSOIL("src/assets/textures/default.png");
        }
        diffuse = defaultTexture;
        specular = defaultTexture;
        normalMap = defaultTexture;
        displacementMap = defaultTexture;
    }

    // Constructor with custom values
    Texture(const char *diffusePath, const char *specularPath)
    {
        diffuse = loadSOIL(diffusePath);
        specular = loadSOIL(specularPath);
    }
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
    bool useNormalMap = false;
    bool useDisplacementMap = false;

    vector<Object *> subObjects;

    Object(){};
    Object(Drawable *drawable);
    Object(Drawable *drawable, Material material);

    void rotateObject(vec3 axis, float angle);
    void scaleObject(vec3 amount);
    void translateObject(vec3 translation);

    void render(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation[3]);
    void render(GLuint modelMatrixLocation);
};