#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

class Object
{
public:
    Drawable *drawable;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mat4 modelMatrix = mat4(1.0f);
    GLuint texture;
    vec3 color;

    Object(){};
    Object(string obj_path, string texture_path, vec3 position, vec3 rotation, vec3 scale)
        : position(position), rotation(rotation), scale(scale)
    {
        drawable = new Drawable(obj_path);
        texture = loadSOIL(texture_path.c_str());
    }
    Object(string obj_path, vec3 color, vec3 position, vec3 rotation, vec3 scale)
        : color(color), position(position), rotation(rotation), scale(scale)
    {
        drawable = new Drawable(obj_path);
    }

    void rotateObject(vec3 axis, float angle)
    {
        mat4 rotationMatrix = rotate(mat4(1.0f), angle, axis);
        rotation = vec3(rotationMatrix * vec4(rotation, 1.0f));
        modelMatrix = rotationMatrix * modelMatrix;
    }

    void scaleObject(vec3 amount)
    {
        mat4 scaleMatrix = glm::scale(mat4(1.0f), amount);
        this->scale = vec3(scaleMatrix * vec4(this->scale, 1.0f));
        modelMatrix = scaleMatrix * modelMatrix;
    }

    void translateObject(vec3 translation)
    {
        mat4 translationMatrix = translate(mat4(1.0f), translation);
        position = vec3(translationMatrix * vec4(position, 1.0f));
        modelMatrix = translationMatrix * modelMatrix;
    }

    void draw(GLuint MLocation, GLuint colorLocation)
    {
        mat4 mainRoomM = modelMatrix;
        drawable->bind();
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &mainRoomM[0][0]);
        glUniform3f(colorLocation, 1.0, 0.5, 0.5);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        drawable->draw();
    }
};