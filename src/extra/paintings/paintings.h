#include <common/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "../player/player.h"

class Painting
{
public:
    float height, width;
    vec3 color;
    vector<vec3> vertices;
    vector<vec3> normals;
    mat4 modelMatrix;
    mat4 mainRoomModelMatrix;
    mat4 sideRoomModelMatrix = translate(mat4(1.0f), vec3(0, height/1.75, 0));
    bool visible = true;

    vec3 boundingBox[8] = {
        vec3(-width / 2, -height / 2, 0),
        vec3(width / 2, -height / 2, 0),

        vec3(-width / 2, height / 2, 0),
        vec3(width / 2, height / 2, 0),
    };


    Drawable *drawable;

    Painting(float height, float width, vec3 color, vector<vec3> vertices) : 
    height(height), width(width), color(color), vertices(vertices)
    {
        vec3 normal = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
        for (int i = 0; i < 6; i++)
        {
            normals.push_back(normal);
        }
        this->drawable = new Drawable(vertices, VEC_VEC2_DEFAUTL_VALUE, normals);
    }
    bool checkCollision(Player *player);
    void draw(GLuint MLocation, GLuint colorLocation);
};

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius);