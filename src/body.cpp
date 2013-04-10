#include "body.h"

Body::Body(const char* model, const char *texture) : Object(model, texture)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    radius = 1;
    position = vec3(0.0, 0.0, 0.0);
    velocity = vec3(0.0, 0.0, 0.0);

    vec3 zero = vec3(0.0, 0.0, 0.0);
    kv1 = zero;
    kv2 = zero;
    kv3 = zero;
    kv4 = zero;
    
    kr1 = zero;
    kr2 = zero;
    kr3 = zero;
    kr4 = zero;

}

Body::Body(const char* model) : Object(model)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    radius = 1;
    position = vec3(0.0, 0.0, 0.0);
    velocity = vec3(0.0, 0.0, 0.0);

}

Body::Body()
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z= 0;
    radius = 1;
    position = vec3(0.0, 0.0, 0.0);
    velocity = vec3(0.0, 0.0, 0.0);
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
