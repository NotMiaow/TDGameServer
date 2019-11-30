#ifndef CORNER_H__
#define CORNER_H__

#include "vector2.h"

struct Corner
{
    Corner() { }
    Corner(const Vector2& position, const int& minDistance) : position(position), minDistance(minDistance) { }
    ~Corner() { }
    Vector2 position;
    int minDistance;
};

#endif