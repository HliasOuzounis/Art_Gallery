#include "room.h"

void Room::render(GLuint shaderProgram, GLuint modelMatrixLocation, GLuint materialLocation[4], GLuint useTextureLocation)
{
    this->light->upload_to_shaders(shaderProgram);
    Object::render(modelMatrixLocation, materialLocation, useTextureLocation);
    for (auto &subObject : subObjects)
        subObject->render(modelMatrixLocation, materialLocation, useTextureLocation);
}

void Room::render(GLuint depthShader, GLuint modelMatrixLocation)
{
    this->light->upload_depth_shader(depthShader);
    Object::render(modelMatrixLocation);
    for (auto &subObject : subObjects)
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

    subObjects.push_back(lightBulbObject);
}