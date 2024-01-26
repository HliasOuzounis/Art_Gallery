#include "object.h"

#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

Object::Object(Drawable *drawable)
{
    this->drawable = drawable;
};

Object::Object(Drawable *drawable, Material material)
{
    this->drawable = drawable;
    this->material = material;
}

void Object::rotateObject(vec3 axis, float angle)
{
    mat4 rotationMatrix = rotate(mat4(1.0f), angle, axis);
    rotation = vec3(rotationMatrix * vec4(rotation, 1.0f));
    modelMatrix = rotationMatrix * modelMatrix;
}

void Object::scaleObject(vec3 amount)
{
    mat4 scaleMatrix = glm::scale(mat4(1.0f), amount);
    scale = vec3(scaleMatrix * vec4(scale, 1.0f));
    modelMatrix = scaleMatrix * modelMatrix;
}

void Object::translateObject(vec3 translation)
{
    mat4 translationMatrix = translate(mat4(1.0f), translation);
    position = vec3(translationMatrix * vec4(position, 1.0f));
    modelMatrix = translationMatrix * modelMatrix;
}

void Object::render(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation)
{
    if (useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.diffuse);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture.specular);

        glUniform1i(useTextureLocation, 1);
    }
    else
    {
        glUniform4fv(materialLocation[0], 1, &material.ambient[0]);
        glUniform4fv(materialLocation[1], 1, &material.diffuse[0]);
        glUniform4fv(materialLocation[2], 1, &material.specular[0]);
        glUniform1f(materialLocation[3], material.shininess);
    }

    drawable->bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    drawable->draw();

    for (int i = 0; i < subObjects.size(); i++)
        subObjects[i]->render(modelMatrixLocation, materialLocation, useTextureLocation);

    glUniform1i(useTextureLocation, 0);
}

void Object::render(GLuint modelMatrixLocation)
{
    drawable->bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    drawable->draw();

    for (int i = 0; i < subObjects.size(); i++)
        subObjects[i]->render(modelMatrixLocation);
}