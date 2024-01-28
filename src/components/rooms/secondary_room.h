#pragma once

#include "room.h"

class SecondaryRoom : public Room
{
public:
    float width;

    SecondaryRoom(float height, float width, float depth);
    bool isInside(vec3 position);
    void addFloor();
    void addCeiling();
private:
    static Texture floorTexture;

    Texture getFloorTexture();
};