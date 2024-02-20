#include "object.h"
#include "src/constants.h"

#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

GLuint Texture::defaultTexture = 0;

Object::Object(Drawable *drawable)
{
    this->drawable = drawable;
    calculateTanBitan();
};

Object::Object(Drawable *drawable, Material material)
{
    this->drawable = drawable;
    this->material = material;
    calculateTanBitan();
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

void Object::render(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation[3])
{
    if (useTexture)
    {
        glActiveTexture(DIFFUSE_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture.diffuse);

        glActiveTexture(SPECULAR_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture.specular);

        glUniform1i(useTextureLocation[0], 1);
    }
    else
    {
        glUniform4fv(materialLocation[0], 1, &material.ambient[0]);
        glUniform4fv(materialLocation[1], 1, &material.diffuse[0]);
        glUniform4fv(materialLocation[2], 1, &material.specular[0]);
        glUniform1f(materialLocation[3], material.shininess);
    }
    if (useNormalMap)
    {
        glActiveTexture(BUMP_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture.normalMap);
        glUniform1i(useTextureLocation[1], 1);
    }
    if (useDisplacementMap)
    {
        glActiveTexture(DISPLACEMENT_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture.displacementMap);
        glUniform1i(useTextureLocation[2], 1);
    }

    drawable->bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    drawable->draw();

    glUniform1i(useTextureLocation[0], 0);
    glUniform1i(useTextureLocation[1], 0);
    glUniform1i(useTextureLocation[2], 0);

    for (int i = 0; i < subObjects.size(); i++)
        subObjects[i]->render(modelMatrixLocation, materialLocation, useTextureLocation);
}

void Object::render(GLuint modelMatrixLocation)
{
    drawable->bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    drawable->draw();

    for (int i = 0; i < subObjects.size(); i++)
        subObjects[i]->render(modelMatrixLocation);
}

void Object::addTexture(Texture newTexture, bool useNormal, bool useDisplacement)
{
    this->texture = newTexture;
    this->useTexture = true;
    this->useNormalMap = useNormal;
    this->useDisplacementMap = useDisplacement;
}

void Object::addDiffuseTexture(GLuint diffuseTexture)
{
    this->texture.diffuse = diffuseTexture;
    this->useTexture = true;
}

void Object::addSpecularTexture(GLuint specularTexture)
{
    this->texture.specular = specularTexture;
    this->useTexture = true;
}

void Object::addNormalTexture(GLuint normalTexture)
{
    this->texture.normalMap = normalTexture;
    this->useNormalMap = true;
}

void Object::addDisplacementTexture(GLuint displacementTexture)
{
    this->texture.displacementMap = displacementTexture;
    this->useDisplacementMap = true;
}

void Object::calculateTanBitan()
{
    vector<float> tangents;
    vector<float> bitangents;

    for (int i = 0; i < this->drawable->vertices.size(); i += 3)
    {
        vec3 pos1 = this->drawable->vertices[i];
        vec3 pos2 = this->drawable->vertices[i + 1];
        vec3 pos3 = this->drawable->vertices[i + 2];

        vec2 uv1 = this->drawable->uvs[i];
        vec2 uv2 = this->drawable->uvs[i + 1];
        vec2 uv3 = this->drawable->uvs[i + 2];

        vec3 edge1 = pos2 - pos1;
        vec3 edge2 = pos3 - pos1;

        vec2 deltaUV1 = uv2 - uv1;
        vec2 deltaUV2 = uv3 - uv1;

        vec3 tangent;
        vec3 bitangent;

        if (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y == 0)
        {
            tangent = vec3(1, 0, 0);
            bitangent = vec3(0, 1, 0);
        }
        else
        {
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        }

        for (int j = 0; j < 3; j++)
        {
            tangents.push_back(tangent.x);
            tangents.push_back(tangent.y);
            tangents.push_back(tangent.z);
            bitangents.push_back(bitangent.x);
            bitangents.push_back(bitangent.y);
            bitangents.push_back(bitangent.z);
        }
    }
    this->drawable->bind();
    glGenBuffers(1, &tangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), &tangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    glGenBuffers(1, &bitangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
    glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(float), &bitangents[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
}