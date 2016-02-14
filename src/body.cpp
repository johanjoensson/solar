#include "body.h"

#include <glm/gtc/type_ptr.hpp>

Body::Body(Model* model, const char *texture) : Object(model, texture)
{
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);
}

Body::Body(const char* model) : Object(model)
{
    position = glm::vec3(0.0, 0.0, 0.0);
    velocity = glm::vec3(0.0, 0.0, 0.0);

}

Body::Body()
{
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
