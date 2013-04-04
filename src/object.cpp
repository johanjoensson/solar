#include "object.h"
#include "GL_utilities.h"
#include <iostream>
using namespace std;

Object::Object(){
    reflectivity = 1;
    matrix = IdentityMatrix();
    rot_mat = IdentityMatrix();
    trans_mat = IdentityMatrix();
}

Object::Object(const char *model)/* : Object()*/
{
    m = LoadModelPlus((char*)model);
    reflectivity = 1;
    matrix = IdentityMatrix();
    rot_mat = IdentityMatrix();
    trans_mat = IdentityMatrix();
}

void Object::draw(int program){
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    DrawModel(m, program, "in_position", "in_normal", NULL);
}

/*Object::~Object(){
    delete vertices;
}*/

void Object::rotate(char direction, float angle)
{
    switch (direction) {
        case 'x':
            // Rotate around x
            rot_mat = Rx(angle) * rot_mat;
            matrix = trans_mat * rot_mat; 
            break;
        case 'y':
            // Rotate around y
            rot_mat = Ry(angle) * rot_mat;
            matrix = trans_mat * rot_mat; 
            break;
        case 'z':
            // Rotate around z
            rot_mat = Rz(angle) * rot_mat;
            matrix = trans_mat * rot_mat; 
            break;
        default:
            cout << "x,y or z" << endl;
    }
}

void Object::translate(float dx, float dy, float dz)
{
    trans_mat = T(dx, dy, dz) * trans_mat;
    matrix = trans_mat * rot_mat;
    //matrix = Mult(T(dx, dy, dz), matrix); // Temporärt för VU3 funkar ej
}

void Object::print_matrix()
{
    cout << matrix.m[0] << " " << matrix.m[1] << " " << matrix.m[2] << " " << matrix.m[3] << endl;
    cout << matrix.m[4] << " " << matrix.m[5] << " " << matrix.m[6] << " " << matrix.m[7] << endl;
    cout << matrix.m[8] << " " << matrix.m[9] << " " << matrix.m[10] << " " << matrix.m[11] << endl;
    cout << matrix.m[12] << " " << matrix.m[13] << " " << matrix.m[14] << " " << matrix.m[15] << endl;
    cout << endl;
}
