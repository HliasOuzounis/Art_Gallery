#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

Drawable *create_room(float height, float radius, int points)
{
    vector<vec3> vertices;

    vec3 center_down = vec3(0, 0, 0);
    vec3 center_up = vec3(0, height, 0);

    for (int i = 0; i < points; i++)
    {
        float angle = i * 2 * PI / points;
        float next_angle = (i + 1) * 2 * PI / points;

        vec3 point1 = vec3(radius * cos(angle), 0, radius * sin(angle)),
             point2 = vec3(radius * cos(next_angle), 0, radius * sin(next_angle)),
             point3 = vec3(radius * cos(angle), height, radius * sin(angle)),
             point4 = vec3(radius * cos(next_angle), height, radius * sin(next_angle));

        vertices.push_back(point1);
        vertices.push_back(center_down);
        vertices.push_back(point2);

        vertices.push_back(point4);
        vertices.push_back(center_up);
        vertices.push_back(point3);

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point1);

        vertices.push_back(point2);
        vertices.push_back(point4);
        vertices.push_back(point3);
    }

    return new Drawable(vertices);
}