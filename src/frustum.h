#pragma once

#include "camera.h"
#include "cel_bodies.h"

class Frustum {
private:
    float near;
    float far;
    float top;
    float bottom;
    float right;
    float left;

    bool inside_frustum(Body p, Camera cam);
public:
    Cel_bodies* cull_frustum(Cel_bodies *world, Camera cam);

    Frustum();
    Frustum(float near_p, float far_p, float bottom_p, float top_p, float left_p, float right_p);
};
