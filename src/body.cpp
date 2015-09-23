#include "body.h"

#include <glm/gtc/type_ptr.hpp>

Body::Body(Model* model, const char *texture) : Object(model, texture)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    set_radius(1);
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);

    glm::vec3 zero = glm::vec3(0.0, 0.0, 0.0);
#ifdef GRAV_OPT
    kv1 = zero;
    kv2 = zero;
    kv3 = zero;
    kv4 = zero;
    
    kr1 = zero;
    kr2 = zero;
    kr3 = zero;
    kr4 = zero;
#endif //GRAV_OPT
}

Body::Body(const char* model) : Object(model)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    set_radius(1);
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);

}

Body::Body()
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z= 0;
    set_radius(1);
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);
}

void Body::draw(int program)
{
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
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

    //TODO RK4 för resten här
    // Position och hastighet, och så att matrix uppdateras
}
