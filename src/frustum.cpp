#include "frustum.h"

Frustum::Frustum()
{
    near = 1.0;
    far = 3.0;
    top = 1.0;
    bottom = -1.0;
    left = -1.0;
    right = -1.0;
}

Frustum::Frustum(float near_p, float far_p, float bottom_p, float top_p, float left_p, float right_p)
{
    near = near_p;
    far = far_p;
    top = top_p;
    bottom = bottom_p;
    left = left_p;
    right = right_p;
}

bool Frustum::inside_frustum(Body p, Camera cam)
{
    vec3 L = Normalize(cam.look_at_pos - cam.position);
    vec3 r = p.position - cam.position;
    vec3 right_vec = Normalize(CrossProduct(L,cam.up));

    vec3 r_L;
    r_L.z = DotProduct(r, L);
    r_L.y = DotProduct(r,cam.up);
    r_L.x = DotProduct(r,right_vec);

    float h = 2*r_L.z*near/top;
    float w = h*top/right;

    if(r_L.z > -near || r_L.z < -far){
        return false;
    }else if(r_L.x < -w/2 || r_L.x < w/2){
        return false;
    }else if(r_L.y < -h/2 || r_L.y > h/2){
        return false;
    }

    return true;
}

Cel_bodies* Frustum::cull_frustum(Cel_bodies *world, Camera cam)
{
    return new Cel_bodies;
}
