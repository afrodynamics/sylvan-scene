#include "Particles.h"

Particles::Particles(int width, int height, int depth) {
    int world_height = height;
    int world_width = width;
    int world_depth = depth;
    for(int i = 0; i < MAX_PARTICLES; i++) {
        Particle p;
        p.xPos = rand() % world_width;
        p.yPos = rand() % world_height;
        p.zPos = (rand() % world_depth) * -1;
        particles.push_back(p);
    }
}

/** rain drop is recreated once it hits the ground */
void Particles::reincarnation(int index) {
    particles[index].xPos = (rand() % 50);
    particles[index].yPos = (rand() % 50);
    particles[index].zPos = (rand() % 50) * (-1);
}

void Particles::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPointSize(3.0);
    glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glEnable(GL_POINT_SPRITE);
    glBegin(GL_POINTS);
    for(int i = 0; i < MAX_PARTICLES;i++) {
        glVertex3f(particles[i].xPos, particles[i].yPos, particles[i].zPos);
    }
    glEnd();
    glDisable(GL_POINT_SPRITE);
    glDisable(GL_TEXTURE_2D);
}

void Particles::update() {
    for(int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].yPos -= 10;
        if(particles[i].yPos <= -50) {
            reincarnation(i);
        }
    }
}
