
// * Screen.cpp

#include "Screen.h"
#include <iostream>
using namespace std;

namespace particlefire {

    // constructor
    Screen::Screen() : m_window(NULL), m_renderer(NULL), m_buffer1(NULL), m_buffer2(NULL) {

    }

    // we are using an init function because our constructor can't return
    // a value to tell us if construction worked or not. We could of course
    // use an exception
    bool Screen::init() {

        // NOTE: NOT SURE WHAT THIS LINE DOES YET.
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }

        // Defines our window
        m_window = SDL_CreateWindow(
            "Particle Fire Explosion", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );

        // make sure the window   
        if(m_window == NULL) {
            SDL_Quit();
            return false;
        }

        // Create a renderer
        // SDL_RENDERER_PRESENTVSYNC is setting it so that our program renders any changes at the 
        // same time as when the monitor is refreshing itself. That way we don't have two things
        // writing to the screen at different times causing "shearing". 
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
        m_texture = SDL_CreateTexture(
            m_renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC, 
            SCREEN_WIDTH, SCREEN_HEIGHT
        );

        if(m_renderer == NULL) {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        if(m_texture == NULL) {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        // we want to allocate enough memory for every pixel visible in the window
        // Every pixel has a Red, Green and Blue as well as an alpha each
        // of which are 8 bits (RGBA8888), which is 32 bits. 
        m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
        m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
        // NOTE: IDEALLY WE SHOULD CHECK TO MAKE SURE THAT THIS BIT OF MEMORY GETS
        // ALLOCATED, B/C IF IT DOESN'T, WE ARE GOING TO CRASH WITHOUT DESTROYING
        // ANY OF OUR OBJECTS. BUT THE IDEAL WAY TO DO THIS IS WITH AN EXCEPTIONG
        // AND I DON'T WANT TO DEAL WITH THAT UNTIL LATER.
        // COME BACK AND DO THIS ONE DAY.

        // write some pixel info into the buffer
        // memset allows you to set a block of memory with a particular value
        memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));      


        return true;
    }

    void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

        // this is not an efficient way to figure out if the pixel is on the 
        // screen. Its a bit of a brute force solution. You should only
        // calculate pixels in range maybe.
        if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
            return;
        }

        // using Uint32 here (and Uint8 elsewhere) because it is gauranteed
        // to be a 32 bit integer on all platforms
        Uint32 color = 0;

        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xff;

        // y * screen width basically moves us down the rows, 
        // and + x moves us right (columns)
        m_buffer1[(y * SCREEN_WIDTH) + x] = color;
    }

    void Screen::update() {
        SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }

    // try to implement a more efficient version of this
    // instead of clearing the screen and redrawing, we are 
    // going to create a whole new screen of pixels, and then 
    // draw that screen.
    void Screen::boxBlur() {

        // swap the buffers, so pixel info is in buffer2 and 
        // we are drawing to buffer1
        Uint32 * temp = m_buffer1;
        m_buffer1 = m_buffer2;
        m_buffer2 = temp;

        // this loop iterates through all the pixels on the screen
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            for(int x = 0; x < SCREEN_WIDTH; x++) {
                /*
                *   0 0 0
                *   0 1 0
                *   0 0 0 
                * 
                *   If the 1 is the pixel we are looking at, we want to add up
                *   the color values in all nine of these pixels, find the average
                *   of these color values, and set this centre pixel to this color
                */
               int redTotal = 0;
               int greenTotal = 0;
               int blueTotal = 0;
               for(int row = -1; row <= 1; row++) {
                   for(int col = -1; col <= 1; col++) {
                       int currentX = x + col;
                       int currentY = y + row;
                        // we need to ignore pixels off the screen
                        if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
                            Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];
                            
                            // get the color values from the RGBA
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;

                            redTotal += red;
                            greenTotal += green;
                            blueTotal += blue;

                        }
                   }
               }

                // here we can find our actual average values for each color (R, G, B)
                // and we can remake a new average total RGBA color
                Uint8 red = redTotal / 9;
                Uint8 green = greenTotal / 9;
                Uint8 blue = blueTotal / 9;

                setPixel(x, y, red, green, blue);
            }
        }
    }

    bool Screen::processEvents(){
        // This loop will run as long as there is an event 
        // waiting to be processed

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            
            // An SDL_Event object has a property type, which 
            // tells you what kind of event has been fired. 
            // So here we are just checking if there any events in 
            // the event queue and then checking if it is the quit
            // event (clicking x) and stopping the program if it is.
            if(event.type == SDL_QUIT) {
                return false;
            }
        }
        return true;
    }
    
    void Screen::close(){
        delete [] m_buffer1;
        delete [] m_buffer2;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
    
}