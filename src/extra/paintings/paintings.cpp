#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>

#include <math.h>
#define PI 3.14159265

using namespace std;
using namespace glm;

#include "paintings.h"

vector<Painting *> createPaintings(int n, float height, float width, float y_pos, float room_radius)
{
    vector<Painting *> paintings;

    vec3 colors[] = {
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(0.5, 0.5, 0),
        vec3(0, 0.5, 0.5),
        vec3(0.5, 0, 0.5),
    };

    float angle;
    float width_angle = atan(width / 2 / room_radius);

    for (int i = 0; i < n; i++)
    {
        vector<vec3> vertices;
        angle = 2 * PI * i / n;

        vec3 pos = vec3(room_radius * cos(angle), y_pos, room_radius * sin(angle));

        float x1 = room_radius * cos(angle - width_angle),
              z1 = room_radius * sin(angle - width_angle),
              x2 = room_radius * cos(angle + width_angle),
              z2 = room_radius * sin(angle + width_angle);

        vec3 point1 = vec3(x1, y_pos - height / 2, z1),
             point2 = vec3(x2, y_pos - height / 2, z2),
             point3 = vec3(x1, y_pos + height / 2, z1),
             point4 = vec3(x2, y_pos + height / 2, z2);

        vertices.push_back(point2);
        vertices.push_back(point3);
        vertices.push_back(point1);

        vertices.push_back(point4);
        vertices.push_back(point3);
        vertices.push_back(point2);

        vec3 color = colors[i];
        Painting *painting = new Painting(height, width, pos, color, vertices);

        paintings.push_back(painting);
    }

    return paintings;
}