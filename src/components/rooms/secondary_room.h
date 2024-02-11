#pragma once

#include "room.h"

class SecondaryRoom : public Room
{
public:
    float width;

    SecondaryRoom(float height, float width, float depth);
void addWalls(float width, float height, float depth);
bool isInside(vec3 position);

private:
void addFloor();
void addCeiling();
};