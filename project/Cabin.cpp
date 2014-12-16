#include "Cabin.h"
#ifndef __APPLE__
#include "glee.h"
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

Cabin::Cabin() {}

Cabin::~Cabin() {}

void Cabin::draw(Matrix4 m) {
    glPushMatrix();
    glLoadMatrixd(m.getPointer());
//    GLUquadricObj *obj = gluNewQuadric();
    glColor3f(0.35, 0.25, 0.2);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(gluNewQuadric(), 5, 5, 20, 100, 100);
    glPopMatrix();
}