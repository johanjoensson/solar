#ifndef SUN_H
#define SUN_H

#include "body.h"
#include "VectorUtils3.h"
class Sun : public Body {
    public:
        vec3 emit_color;
}

#endif
