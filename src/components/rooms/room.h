#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "player/player.h"
#include "floor.h"
#include "object.h"

class Room
{
public:
    vector<vec3> vertices;
    mat4 modelMatrix = mat4(1.0f);
    vec3 roomColor = vec3(0.2f, 0.2f, 0.2f);
    // texture

    GLuint shaderProgram;
    Drawable *drawable;
    float depth = 0;
    float height = 0;
    Floor *floor;
    Floor *ceiling;
    vector<Object *> objects;

    Material material = {
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        1.0f};
    Texture texture;
    bool useTexture = false;

    Room(){};
    virtual bool isInside(vec3 position) { return false; }

    void draw(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation)
    {
        mat4 mainRoomM = modelMatrix;
        drawable->bind();

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mainRoomM[0][0]);

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
        drawable->draw();
        glUniform1i(useTextureLocation, 0);

        floor->draw(modelMatrixLocation, materialLocation, useTextureLocation);
        ceiling->draw(modelMatrixLocation, materialLocation, useTextureLocation);

        for (int i = 0; i < objects.size(); i++)
            objects[i]->draw(modelMatrixLocation, materialLocation, useTextureLocation);
        
    }
    void draw(GLuint modelMatrixLocation)
    {
        drawable->bind();
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        drawable->draw();

        floor->draw(modelMatrixLocation);
        ceiling->draw(modelMatrixLocation);

        for (int i = 0; i < objects.size(); i++)
            objects[i]->draw(modelMatrixLocation);
    }
};

class MainRoom : public Room
{
public:
    float radius;

    MainRoom(float height, float radius, int points);
    bool isInside(vec3 position);
};

class SecondaryRoom : public Room
{
public:
    float width;

    SecondaryRoom(float height, float width, float depth);
    bool isInside(vec3 position);
};