#include "object.h"
#include <iostream>
using namespace std;

Object::Object(){
    reflectivity = 1;
}

Object::~Object(){
    delete vertices;
}

void Object::rotate(char direction, float angle)
{
    switch (direction) {
        case 'x':
            // Rotate around x
            matrix =  Rx(angle) * matrix;
            break;
        case 'y':
            // Rotate around y
            matrix =  Ry(angle) * matrix;
            break;
        case 'z':
            // Rotate around z
            matrix =  Rz(angle) * matrix;
            break;
        default:
            cout << "x,y or z" << endl;
    }
}

void Object::translate(float dx, float dy, float dz)
{
    matrix = T(dx, dy, dz) * matrix; 
}
