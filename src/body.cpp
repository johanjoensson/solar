#include "body.h"

Body::Body()
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z= 0;
    radius = 1;
}

void Body::update(float dt)
{
    rotate('x', dt*spin_x);
    rotate('y', dt*spin_y);
    rotate('z', dt*spin_z);

    //TODO RK4 för resten här
    // Position och hastighet, och så att matrix uppdateras
}
