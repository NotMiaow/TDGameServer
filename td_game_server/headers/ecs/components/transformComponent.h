#ifndef TRANSFORM_COMPONENT_H__
#define TRANSFORM_COMPONENT_H__

#include "vector2.h"

struct TransformComponent
{
    TransformComponent() { }
    TransformComponent(const Vector2& position) : position(position) { }

    Vector2 position;
    //Vector3 scale;
};

#endif