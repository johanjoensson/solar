#include "frustum.h"
#include <iostream>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

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
    glm::vec3 L = normalize(cam.look_at_pos - cam.position);
    glm::vec3 r = p.position - cam.position;
    glm::vec3 up = normalize(cam.up);
    glm::vec3 right_vec = cross(L,up);

    float radius = p.get_radius();

    glm::vec3 r_L;

    r_L.z = -dot(r, L);
    r_L.y = dot(r,up);
    r_L.x = dot(r,right_vec);

    float h = -r_L.z*top/near;
    float w = h*right/top;

    if(r_L.z > -near || r_L.z < -far - radius){
        return false;
    }else if(r_L.y < -h - radius || r_L.y > h + radius){
    /* Så länge upp-vektorn inte räknas ut korrekt bör vi skippa frustum culling i höjdled */
        return true;
//        return false;
    }else if(r_L.x < -w - radius || r_L.x > w + radius){
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
