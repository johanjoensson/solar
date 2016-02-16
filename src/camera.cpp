#include "camera.h"
#include <cmath>
#include <iostream>
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace std;

Camera::Camera(std::vector<GLuint> shaders) :
    shaders(std::move(shaders))
{
    x = 0;
    y = 0;
    position = glm::vec3(0,0,-50);
    look_at_pos = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    matrix = glm::lookAt(position, look_at_pos, up);
    upload();
}

void Camera::upload()
{
    cam_position[0] = position.x;
    cam_position[1] = position.y;
    cam_position[2] = position.z;
        
    // Upload camera matrix here
    for (const auto &shader : shaders)
    {
        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "cam_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
        glUniform3fv(glGetUniformLocation(shader, "camera_pos"), 1, (const GLfloat*) (cam_position));
    }
}

void Camera::update()
{
    matrix = glm::lookAt(position, look_at_pos, up);
    upload();
}

void Camera::rotate(char direction, float angle)
{
    switch (direction){
        case 'x':
            look_at_pos = glm::rotateX(look_at_pos, angle);
            break;
        case 'y':
            look_at_pos = glm::rotateY(look_at_pos, angle);
            break;
        case 'z':
            look_at_pos = glm::rotateZ(look_at_pos, angle);
            break;
    }
    matrix = glm::lookAt(position, look_at_pos, up);
    upload();
}

void Camera::translate(glm::vec3 d_pos)
{
    position = d_pos + position;
    look_at_pos = d_pos + look_at_pos;
    matrix = glm::lookAt(position, look_at_pos, up);
    upload();
}

void Camera::forward(float d)
{
    glm::vec3 forward_vec = look_at_pos - position;
    forward_vec = d * forward_vec;
    translate(forward_vec);
}

void Camera::strafe(float d)
{
    glm::vec3 strafe_vec = look_at_pos - position;
    strafe_vec = cross(up, strafe_vec);
    strafe_vec = normalize(strafe_vec);
    strafe_vec = d * strafe_vec;
    translate(strafe_vec);
}

void Camera::change_look_at_pos(int xrel, int yrel, int width, int height)
{
    // FIXME ful lösning är detta
    y += yrel;
    if(y <= 0){
        y = 1;
    } else if (y >= height) {
        y = height - 1;
    }
    x += xrel;
    float fi = ((float)x)/width*2*M_PI;
    float theta = ((float)y)/height*M_PI;

    look_at_pos.x = -sin(theta)*sin(fi) + position.x;
    look_at_pos.y = cos(theta) + position.y;
    look_at_pos.z = sin(theta)*cos(fi) + position.z;
    matrix = glm::lookAt(position, look_at_pos, up);
    upload();
}
