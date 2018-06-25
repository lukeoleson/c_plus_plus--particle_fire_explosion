/* Manages a swarm of particles */

/* Defines a particle */

#ifndef SWARM_H_
#define SWARM_H_

#include "Particle.h"

namespace particlefire {
    
    class Swarm {

        public:
        const static int NPARTICLES = 5000;

        private: 
            Particle* m_pParticles;
            int lastTime;

        public:
            Swarm();
            virtual ~Swarm();

            // the particle cannot be changed with this pointer, and this 
            // pointer cannot be pointed to anything else
            const Particle* const getParticles() { return m_pParticles; };

            void update(int elapsed);

    };
}

#endif
