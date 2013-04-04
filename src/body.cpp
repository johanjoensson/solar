#include "body.h"

Body::Body(const char* model, const char *texture) : Object(model, texture)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    radius = 1;
}

Body::Body(const char* model) : Object(model)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
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

void Body::draw(int program)
{
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
}

void Body::update(float dt)
{
    this->rotate('x', dt*spin_x);
    this->rotate('y', dt*spin_y);
    this->rotate('z', dt*spin_z);

    //TODO RK4 för resten här
    // Position och hastighet, och så att matrix uppdateras
}
