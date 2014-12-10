#include "Particles.h"

Particles::Particles(int width, int height, int depth) {
    int world_height = height;
    int world_width = width;
    int world_depth = depth;
    isActive = false;
    for(int i = 0; i < MAX_PARTICLES; i++) {
        Particle p;
        particles.push_back(p);
        particles[i].xPos = rand() % world_width;
        particles[i].yPos = rand() % world_height;
        particles[i].zPos = rand() % world_depth;
    }
}

/** rain drop is recreated once it hits the ground */
void Particles::reincarnation(int index) {
    particles[index].xPos = rand() % world_width;
    particles[index].yPos = rand() % world_height;
    particles[index].zPos = rand() % world_depth;
}

void Particles::render() {
    if(isActive) {
        for(int i = 0; i < MAX_PARTICLES;i++) {
            glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
            glEnable(GL_POINT_SPRITE);
            glBegin(GL_POINTS);
            glVertex3f(particles[i].xPos, particles[i].yPos, particles[i].zPos);
            glEnd();
            glDisable(GL_POINT_SPRITE);
        }
    }
}

void Particles::update() {
    if(isActive) {
        for(int i = 0; i < MAX_PARTICLES; i++) {
            particles[i].yPos -= (rand() % 2) - 2.5;
            if(particles[i].yPos <= 0) {
                reincarnation(i);
            }
        }
    }
}

void Particles::setActive(bool val) {
    isActive = val;
}