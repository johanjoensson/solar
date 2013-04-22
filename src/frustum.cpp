#include "frustum.h"
#include <iostream>
#include <cmath>

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
    vec3 up = Normalize(cam.up);
    vec3 right_vec = CrossProduct(L,up);

    vec3 r_L;

    r_L.z = -DotProduct(r, L);
    r_L.y = DotProduct(r,up);
    r_L.x = DotProduct(r,right_vec);

    float h = -r_L.z*top/near;
    float w = h*right/top;

    if(r_L.z > -near || r_L.z < -far){
        return false;
    }else if(r_L.y < -h || r_L.y > h){
        return false;
    }else if(r_L.x < -w || r_L.x > w){
        return false;

    }

    return true;
}

Cel_bodies* Frustum::cull_frustum(Cel_bodies *world, Camera cam)
{
    Cel_bodies *current = world;
    Cel_bodies sentinel;
    while(current != NULL){
        if(inside_frustum(*current->planet, cam)){
            sentinel.add_planet(current->planet);
        }
        current = current->next;
    }
    return sentinel.next;
}
