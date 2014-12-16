#include "Particles.h"
#include "Util.h"

Particles::Particles(int width, int height, int depth) {
    // Removed type declarations. Now arguments will be saved
    // in the particle system, which is what I assume you wanted. ~ Dexter
    world_height = height;
    world_width = width;
    world_depth = depth;
    world_floor = -height / 2;
    for(int i = 0; i < MAX_PARTICLES; i++) {
        Particle p;
        p.xPos = (rand() % world_width) - world_width / 2;
        p.yPos = (rand() % world_height) - world_height / 2;
        p.zPos = (rand() % world_depth) - world_depth / 2;
        particles.push_back(p);
    }
}

/** snow particle is recreated once it hits the ground */
void Particles::reincarnation(int index) {
    particles[index].xPos = (rand() % world_width / 2) * ( Util::drand() > .5 ? -1 : 1 );// - world_width / 4;
    particles[index].yPos = (rand() % world_height / 2) * ( Util::drand() ); // - world_height / 4;
    particles[index].zPos = (rand() % world_depth / 2) * ( Util::drand() > .5 ? -1 : 1 );// - world_depth / 4;
}

void Particles::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glEnable(GL_POINT_SPRITE);

    glPointSize(2.5);
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_POINTS);
    for(int i = 0; i < MAX_PARTICLES; i++) {
        glVertex3f(particles[i].xPos, particles[i].yPos, particles[i].zPos);
    }
    glEnd();

    glDisable(GL_POINT_SPRITE);
    // update(); // This is now being done in Window::idlecallback()
}

void Particles::update() {
    for(int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].xPos -= (rand() % 3);
        particles[i].yPos -= (rand() % 10);
        particles[i].zPos -= (rand() % 3);
        if (particles[i].yPos <= world_floor) {
            reincarnation(i);
        }
    }
}
