#include "Particle.h"
#include <stdlib.h>
#include <math.h>

namespace particlefire {

    Particle::Particle() {

        m_x = 0;
        m_y = 0;

        // this is not a cartesian plane. It is a 
        // polar plane, hence why we are using 2 pi
        // here. Our direction is in fact an angle.
        m_direction = (2 * M_PI * rand()) / RAND_MAX;
        m_speed = (0.0001 * rand()) / RAND_MAX;
    }

    Particle::~Particle() {
        // TODO Auto generated desctructor stub
    }

    void Particle::update(int interval) {

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
    }

}