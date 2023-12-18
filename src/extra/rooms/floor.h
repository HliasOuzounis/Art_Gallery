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
    vec3 floorColor = vec3(0.1f, 0.11f, 0.1f);
    // texture
    Drawable *drawable;
    float width, depth;
    Floor(float width, float depth, float height, bool isCeiling)
    {
        vertices.push_back(vec3(width, height, -depth));
        vertices.push_back(vec3(-width, height, depth));
        vertices.push_back(vec3(-width, height, -depth));

        vertices.push_back(vec3(-width, height, depth));
        vertices.push_back(vec3(width, height, depth));
        vertices.push_back(vec3(width, height, -depth));

        vec3 normal = vec3(0, 1, 0);
        if (isCeiling)
            normal = -normal;
        for (int i = 0; i < 6; i++)
            normals.push_back(normal);

        this->drawable = new Drawable(vertices, uvs, normals);
    };

    void draw(GLuint MLocation, GLuint colorLocation)
    {
        drawable->bind();
        glUniformMatrix4fv(MLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3f(colorLocation, floorColor.x, floorColor.y, floorColor.z);
        drawable->draw();
    }
};