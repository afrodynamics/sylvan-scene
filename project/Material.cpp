#include "Material.h"

/* Default constructor
 * sets diffuse to {0.8,0.8,0.8,1}, shininess to 1,
 * specular to {1,1,1,1}, ambient to {0.2,0.2,0.2,1} and color to white
 */
Material::Material() {
  shininess = 1;
  
  for(int i = 0; i < 4; ++i) {
    ambient[i] = 0.2;
    diffuse[i] = 0.8;
    specular[i] = 1;
    color[i] = 1;
  }
  ambient[3] = 1;
  diffuse[3] = 1;
}

// Set shininess value
Material Material::setShine(float s) {
  if( s < 0 ) s = 0;

  shininess = s;
  return *this;
}

// Set ambient values.  Length is the length of array a
Material Material::setAmbient(float a[], int length) {
  for( int i = 0; i < length; i++ ) {
    if( a[i] < 0 ) a[i] = 0;
    if( a[i] > 1 ) a[i] = 1;
    ambient[i] = a[i];
  }
  for( int i = length; i < 4; i++ ) {
    if(i == 3) {
      ambient[i] = 1;
      break;
    }
    ambient[i] = 0;
  }

  return *this;
}

// Set diffuse values.  Length is the length of array d
Material Material::setDiffuse(float d[], int length) {
  for( int i = 0; i < length; i++ ) {
    if( d[i] < 0 ) d[i] = 0;
    if( d[i] > 1 ) d[i] = 1;
    diffuse[i] = d[i];
  }
  for( int i = length; i < 4; i++ ) {
    if(i == 3) {
      diffuse[i] = 1;
      break;
    }
    diffuse[i] = 0;
  }

  return *this;
}

// Set specular values.  Length is the length of array s
Material Material::setSpecular(float s[], int length) {
  for( int i = 0; i < length; i++ ) {
    if( s[i] < 0 ) s[i] = 0;
    if( s[i] > 1 ) s[i] = 1;
    specular[i] = s[i];
  }
  for( int i = length; i < 4; i++ ) {
    if(i == 3) {
      specular[i] = 1;
      break;
    }
    specular[i] = 0;
  }

  return *this;
}


// Set color of material.  Length is the length of array c
Material Material::setColor(float c[], int length) {
  for( int i = 0; i < length; i++ ) {
    if( c[i] < 0 ) c[i] = 0;
    if( c[i] > 1 ) c[i] = 1;
    color[i] = c[i];
  }
  for( int i = length; i < 4; i++ ) {
    if(i == 3) {
      specular[i] = 1;
      break;
    }
    color[i] = 0;
  }

  return *this;
}

// Apply the material before drawing
void Material::apply() {
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glColor4fv(color);
}