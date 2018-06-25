#include "Particle.h"
#include <stdlib.h>
#include <math.h>

namespace particlefire {

    Particle::Particle() {

        init();

        
    }

    Particle::~Particle() {
        // TODO Auto generated desctructor stub
    }

    void Particle::init() {

         m_x = 0;
         m_y = 0;

        // this is not a cartesian plane. It is a 
        // polar plane, hence why we are using 2 pi
        // here. Our direction is in fact an angle.
        m_direction = (2 * M_PI * rand()) / RAND_MAX;
        m_speed = (0.04 * rand()) / RAND_MAX;

        // we are squaring the speed here so that 
        // we get a less uniform bunch of particles
        // at the beggining of the explosion
        m_speed *= m_speed;
    }

    void Particle::update(int interval) {

        // adding some curl to the particles
        m_direction += interval * 0.0003;

        // these formulas give us our speed in the right
        // direction. Try to figure out why exactly later.
        double xspeed = m_speed * cos(m_direction);
        double yspeed = m_speed * sin(m_direction);

        // sets the speed for a particle
        // we multiple by the interval to ensure that the 
        // speed that the particle is moving at is relative
        // to the amount of time that has elapsed between
        // updates. This is so that things run at a similar 
        // speed on all different speed systems. 
        m_x += xspeed * interval;
        m_y += yspeed * interval;

        // for the curl, 
        // if the particle goes off the screen, reinitialze it
        // and put it back at the center of the screen. 
        if(m_x < -1 || m_x > 1 ||m_y < -1 || m_y > 1) {
            this->init();
        }

        // true 1 in every 100 times.
        if(rand() < RAND_MAX / 100) {
            init();
        }
    }

}