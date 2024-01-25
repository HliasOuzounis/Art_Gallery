#pragma once

#include <common/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "player/player.h"
#include "rooms/object.h"

class Painting
{
public:
    float height, width;
    vec3 color;
    vector<vec3> vertices;
    vector<vec3> normals;
    mat4 modelMatrix;
    mat4 mainRoomModelMatrix;
    bool visible = true;
    Object *frame;
    Material material;
    Texture texture;
    bool useTexture = false;

    vec3 boundingBox[8] = {
        vec3(-width / 2, -height / 2, 0),
        vec3(width / 2, -height / 2, 0),

        vec3(-width / 2, height / 2, 0),
        vec3(width / 2, height / 2, 0),
    };

    Drawable *drawable;

    Painting(float height, float width, Material material, vector<vec3> vertices) : height(height), width(width), material(material), vertices(vertices)
    {
        vec3 normal = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
        for (int i = 0; i < 6; i++)
        {
            normals.push_back(normal);
        }
        this->drawable = new Drawable(vertices, VEC_VEC2_DEFAUTL_VALUE, normals);
    }
    void draw(GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation)
    {
        if (!visible)
            return;

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

        glUniform1i(useTextureLocation, 0);
        frame->draw(modelMatrixLocation, materialLocation, useTextureLocation);
    }

    void draw(GLuint modelMatrixLocation)
    {
        if (!visible)
            return;
        drawable->bind();
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        drawable->draw();
        frame->draw(modelMatrixLocation);
    }

    bool checkCollision(Player *player);
    void update_frame_model_matrix();
};

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius);
