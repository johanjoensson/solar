#include "body.h"

Body::Body(const char* model) : Object(model)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z= 0;
    radius = 1;
}

Body::Body()
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z= 0;
    radius = 1;
}

vec3 Body::set_acceleration(float h)
{
    GLfloat res = 1.0;
    return vec3(res,res,res);
}

void Body::set_velocity(float h)
{
    velocity = velocity + h*set_acceleration(h);
    return;
}

void Body::set_position(float h)
{
    set_velocity(h);
    position = position + h*velocity;
    return;
}

void Body::gravity(float h)
{
    set_position(h);
}

void Body::update(float dt)
{
    this->rotate('x', dt*spin_x);
    this->rotate('y', dt*spin_y);
    this->rotate('z', dt*spin_z);

    this->gravity(dt);
    //TODO RK4 för resten här
    // Position och hastighet, och så att matrix uppdateras
}
