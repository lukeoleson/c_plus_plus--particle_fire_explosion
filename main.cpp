// might have to use the -L flag to tell the compiler where to look for some libraries.

#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Screen.h"
#include "Swarm.h"
#include "Particle.h"

using namespace std;
using namespace particlefire;

/*
update the texture with the info in the buffer (pixel info)
clear the renderer
copy the texture to the renderer
present the renderer on the screen
*/
int main() {
        
    srand(time(NULL));

    Screen screen;

    if(screen.init() == false) {
        cout << "Error initializing SDL." << endl;
    }

    Swarm swarm;

    // The Game Loop
    while(true) {
        // Update particle positions
        // draw particles

        // Here we want to get numbers that we can use to animate our
        // our colors. For this we want a smooth transition between numbers
        // in a certain range (0 to 255).
        // We are going to use sin to get numbers between -1 and +1 by feeding
        // it an ever increasing number - the number of msec since the program
        // started
        // returns the number of msec since the program started
        int elapsed = SDL_GetTicks();

        // we took out the clear here to add the blur instead. 
        // screen.clear();
        swarm.update(elapsed);

        // sin actually returns a double, but we only want the int portion.
        // this means we could use an int to only grab the integer portion, 
        // but we will actually use an unsigned char since it will only accept
        // a number between 0 and 255 even if somehow we try to get a 256 in
        // there. 
        unsigned char red = (1 + sin(elapsed * 0.0001)) * 128;
        unsigned char green = (1 + sin(elapsed * 0.0002)) * 128;
        unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;

        // read this backwards - "const pointer to a particle that is const"
        const Particle * const pParticles = swarm.getParticles();
        // loop through each particle in the swarm
        for (int i = 0; i < Swarm::NPARTICLES; i++) {
            // get the individual particles one by one
            Particle particle = pParticles[i];

            // first adjust it to range from 0 to 2
            // then put it in the range of the screen
            // NOTE: might be more efficient to define this divided by 2 as a constant outside this loop
            int x = (particle.m_x + 1) * Screen::SCREEN_WIDTH / 2;
            int y = particle.m_y * Screen::SCREEN_WIDTH / 2 + Screen::SCREEN_HEIGHT / 2;

            screen.setPixel(x, y, red, green, blue);
        }

        screen.boxBlur();
        
        // Draw the Screen
        screen.update();

        if(screen.processEvents() == false) {
            break;
        }
    }

    screen.close();

    // Dispose of any memory we have created
    
    return 0;
}