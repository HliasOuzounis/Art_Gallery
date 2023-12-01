#include <common/model.h>
#include <glm/glm.hpp>
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
    mat4 modelMatrix;
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
        this->drawable = new Drawable(vertices);
    }
    bool checkCollision(Player *player);
};

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius);