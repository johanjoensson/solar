#ifndef SUN_H
#define SUN_H

#include "body.h"
#include "VectorUtils3.h"
class Sun : public Body 
{
 private:
 public:
  vec3 emit_color;
  vec3 sun_position;
  //FIXME
  float specularExponent;
  void draw(int);
  Sun(const char*, const char*, const float);
  Sun(const char*, const char*);

};

#endif
