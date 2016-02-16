#include "body.h"

#include <glm/gtc/type_ptr.hpp>

Body::Body(Model* model, const char *texture, GLuint shader) : Object(model, texture, shader)
{
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);
}

void Body::set_radius(float r)
{
    set_scale(r);
}

float Body::get_radius()
{
    return get_scale();
}

void Body::update(float dt)
{
    this->rotate('x', dt*spin_x);
    this->rotate('y', dt*spin_y);
    this->rotate('z', dt*spin_z);
}
