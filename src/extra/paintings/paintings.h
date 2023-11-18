#include <common/model.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Painting
{
public:
    vec3 position;
    float height, width;
    vec3 color;
    vector<vec3> vertices;

    Drawable *drawable;

    Painting(float height, float width, vec3 position, vec3 color, vector<vec3> vertices) : 
    height(height), width(width), position(position), color(color), vertices(vertices)
    {
        this->drawable = new Drawable(vertices);
    }
};

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius);