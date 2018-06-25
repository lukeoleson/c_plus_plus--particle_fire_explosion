#include "Swarm.h"

namespace particlefire {

    // this constructor syntax is called a "constructor
    // initialize list" or "inline initialization". It
    // is equivalent to doing it in the function body.
    Swarm::Swarm(): lastTime(0) {
        m_pParticles = new Particle[NPARTICLES];
    }

    Swarm::~Swarm() {
        delete[] m_pParticles;
    }

    void Swarm::update(int elapsed) {

        // get the interval between updates
        int interval = elapsed - lastTime;

        // it would be more efficient to do this in main.cpp loop
        for (int i = 0; i < Swarm::NPARTICLES; i++) {
            m_pParticles[i].update(interval);
        }

        // store the time of this update
        lastTime = elapsed; 

    }

}