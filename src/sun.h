#ifndef SUN_H
#define SUN_H

#include "body.h"
#include "VectorUtils3.h"
class Sun : public Body 
{
 private:
 public:
  GLfloat emit_color[3];
  GLfloat sun_position[3];
  const GLfloat specularExponent = 10.0;
  void draw(int);
  Sun(const char*, const char*, const float);
  Sun(const char*, const char*);

};

#endif
