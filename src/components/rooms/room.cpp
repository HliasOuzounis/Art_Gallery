#include "room.h"

void Room::render(GLuint shaderProgram, GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation[4])
{
    this->light->upload_to_shaders(shaderProgram);
    for (auto &subObject : roomObjects)
        subObject->render(modelMatrixLocation, materialLocation, useTextureLocation);
}

void Room::render(GLuint depthShader, GLuint modelMatrixLocation)
{
    this->light->upload_depth_shader(depthShader);
    for (auto &subObject : roomObjects)
        subObject->render(modelMatrixLocation);
}

void Room::render(GLuint modelMatrixLocation)
{
    for (auto &subObject : roomObjects)
        subObject->render(modelMatrixLocation);
}

void Room::addLightBulb()
{
    Drawable *lightBulb = new Drawable("src/assets/models/sphere.obj");
    vector<vec3> normals;
    for (auto &n : lightBulb->normals)
        normals.push_back(-n);
    lightBulb = new Drawable(lightBulb->vertices, lightBulb->uvs, normals);

    Material white = {
        vec4(0.1f, 0.1f, 0.1f, 1.0f),
        vec4(0.5f, 0.5f, 0.5f, 1.0f),
        vec4(0.5f, 0.5f, 0.5f, 1.0f),
        32.0f};

    Object *lightBulbObject = new Object(lightBulb, white);
    lightBulbObject->scaleObject(vec3(0.1f));
    lightBulbObject->translateObject(this->light->position);

    roomObjects.push_back(lightBulbObject);
}