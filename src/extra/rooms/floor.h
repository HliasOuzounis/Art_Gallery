#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Floor
{
public:
    vector<vec3> vertices;
    vector<vec2> uvs = VEC_VEC2_DEFAUTL_VALUE;
    vector<vec3> normals;
    mat4 modelMatrix = mat4(1.0f);
    vec3 floorColor = vec3(1.0f, 0.1f, 0.1f);
    // texture
    Drawable *drawable;
    float width, depth;
    Floor(float width, float depth, float height, bool isCeiling)
    {
        vec3 point1 = vec3(-width, height, depth);
        vec3 point2 = vec3(-width, height, -depth);
        vec3 point3 = vec3(width, height, depth);
        vec3 point4 = vec3(width, height, -depth);

        vec3 normal = vec3(0, 1, 0);
        if (isCeiling){
            normal = -normal;
            swap(point2, point3);
        }
        vertices.push_back(point2);
        vertices.push_back(point1);
        vertices.push_back(point3);

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point4);

        for (int i = 0; i < 6; i++)
            normals.push_back(normal);

        this->drawable = new Drawable(vertices, uvs, normals);
    };

    void draw(GLuint MLocation, GLuint colorLocation=-1)
    {
        drawable->bind();
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (colorLocation != -1)
            glUniform3f(colorLocation, floorColor.x, floorColor.y, floorColor.z);
        drawable->draw();
    }
};