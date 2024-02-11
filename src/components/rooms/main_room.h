#pragma once

#include "room.h"

class MainRoom : public Room
{
public:
    float radius;

    MainRoom(float height, float radius, int points);
    bool isInside(vec3 position);

private:
    void addFloor();
    void addCeiling();
};
