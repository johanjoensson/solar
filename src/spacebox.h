#ifndef SPACEBOX_H
#define SPACEBOX_H

#include "object.h"
#include "LoadTGA.h"

class Spacebox : public Object {
    public:
        void draw(int);
        Spacebox();
        Spacebox(const char*, const char*);
};
#endif
