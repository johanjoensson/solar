#include "object.h"
#include "GL_utilities.h"
#include <iostream>
using namespace std;

Object::Object(){
    reflectivity = 1;
    matrix = IdentityMatrix();
}

Object::Object(const char *model)
{
    m = LoadModelPlus((char*)model);
    reflectivity = 1;
    matrix = IdentityMatrix();
}

void Object::draw(int program){
    DrawModel(m, program, (char*)"in_Position", NULL, NULL);
}

/*Object::~Object(){
    delete vertices;
}*/

void Object::rotate(char direction, float angle)
{
    switch (direction) {
        case 'x':
            // Rotate around x
            matrix = Mult(Rx(angle), matrix); // Temporärt för VU3 funkar ej
            //matrix = Ry(angle) * matrix;
            break;
        case 'y':
            // Rotate around y
            matrix = Mult(Ry(angle), matrix); // Temporärt för VU3 funkar ej
            //matrix = Ry(angle) * matrix;
            break;
        case 'z':
            // Rotate around z
            matrix = Mult(Rz(angle), matrix); // Temporärt för VU3 funkar ej
            //matrix = Rz(angle) * matrix;
            break;
        default:
            cout << "x,y or z" << endl;
    }
}

void Object::translate(float dx, float dy, float dz)
{
    matrix = Mult(T(dx, dy, dz), matrix); // Temporärt för VU3 funkar ej
}

void Object::print_matrix()
{
    cout << matrix.m[0] << " " << matrix.m[1] << " " << matrix.m[2] << " " << matrix.m[3] << endl;
    cout << matrix.m[4] << " " << matrix.m[5] << " " << matrix.m[6] << " " << matrix.m[7] << endl;
    cout << matrix.m[8] << " " << matrix.m[9] << " " << matrix.m[10] << " " << matrix.m[11] << endl;
    cout << matrix.m[12] << " " << matrix.m[13] << " " << matrix.m[14] << " " << matrix.m[15] << endl;
    cout << endl;
}
