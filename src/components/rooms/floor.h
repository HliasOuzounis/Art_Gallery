#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

#include <common/model.h>
#include <common/texture.h>

using namespace glm;
using namespace std;

#include "object/object.h"

class Floor
{
public:
    vector<vec3> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;
    Drawable *drawable;

    mat4 modelMatrix = mat4(1.0f);

    Material material = {
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        1.0f};
    Texture texture;
    bool useTexture = false;

    float width, depth;

    Floor(float width, float depth, float height, bool isCeiling)
    {
        vec3 point1 = vec3(-width, height, depth);
        vec3 point2 = vec3(-width, height, -depth);
        vec3 point3 = vec3(width, height, depth);
        vec3 point4 = vec3(width, height, -depth);

        vec3 normal = vec3(0, 1, 0);
        if (isCeiling)
        {
            normal = -normal;
            swap(point2, point3);
        }
        vertices.push_back(point2);
        vertices.push_back(point1);
        vertices.push_back(point3);
        uvs.push_back(vec2(0, 0));
        uvs.push_back(vec2(1, 0));
        uvs.push_back(vec2(1, 1));

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point4);
        uvs.push_back(vec2(0, 0));
        uvs.push_back(vec2(1, 1));
        uvs.push_back(vec2(0, 1));

        for (int i = 0; i < 6; i++)
            normals.push_back(normal);

        this->drawable = new Drawable(vertices, uvs, normals);
    };

    void draw(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation)
    {
        if (useTexture){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.diffuse);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture.specular);

            glUniform1i(useTextureLocation, 1);
        } else {
            glUniform4fv(materialLocation[0], 1, &material.ambient[0]);
            glUniform4fv(materialLocation[1], 1, &material.diffuse[0]);
            glUniform4fv(materialLocation[2], 1, &material.specular[0]);
            glUniform1f(materialLocation[3], material.shininess);
        }

        draw(modelMatrixLocation);

        glUniform1i(useTextureLocation, 0);
    }

    void draw(GLuint modelMatrixLocation)
    {
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        
        drawable->bind();
        drawable->draw();
    }
};