#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdlib.h>
#ifndef __APPLE__
#include "glee.h"
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Material {
  public:
    Material();

    void apply(); // Apply the material before drawing

    // All return self for easy setting
    Material setShine(float);            // Set shininess value
    Material setAmbient(float[], int); // Set specular values
    Material setAmbient(float, float, float, float); // Set specular values
    Material setDiffuse(float[], int);   // Set diffuse values
    Material setDiffuse(float, float, float, float);   // Set diffuse values
    Material setSpecular(float[], int); // Set specular values
    Material setSpecular(float, float, float, float); // Set specular values
    Material setColor(float[], int);    // Set color of material
    Material setColor(float, float, float, float);    // Set color of material

  private:
    float shininess;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float color[4];

};

#endif
