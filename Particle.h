/* Defines a particle */

#ifndef PARTICLE_H_
#define PARTICLE_H_

namespace particlefire {
    
    // the only difference between a class and a struct 
    // is that the members are public by default. 
    // Here we want public members for the sake of efficiency
    // here since we need to access so many particle positions
    // so often. This is common in game development; you have
    // to break encapsulation in order to program efficiently.
    struct Particle {
        
            // member variables for particle position
            // we are using double's here so that we can do finer
            // gradations in terms of moving a particle from pixel
            // to pixel. Of course, we can only actual move to a new
            // pixel by integer values, but our code will use finer
            // values to kind of conceptually move them more gradually.
            double m_x;
            double m_y;

            double m_speed;
            double m_direction;

        public:
            Particle();
            virtual ~Particle();
            void update(int interval);
    };
}

#endif
