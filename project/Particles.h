#define MAX_PARTICLES 1000
#include "Vector3.h"
#include <vector>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

struct Particle
{
    int xPos;
    int yPos;
    int zPos;
};

class Particles {
private:
    int world_height;
    int world_width;
    int world_depth;
    std::vector<Particle> particles;

public:
    Particles(int width, int height, int depth);
    void create();
    void update();
    void reincarnation(int index);
    void render();
};