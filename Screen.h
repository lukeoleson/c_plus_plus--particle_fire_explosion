// * Screen.h

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>

namespace particlefire {

class Screen {

    public: 
        const static int SCREEN_WIDTH = 800;
        const static int SCREEN_HEIGHT = 600;
        
    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_texture;
        Uint32* m_buffer1;
        Uint32* m_buffer2;

    public:
        
        // constructor
        Screen();
        bool init();
        void update();
        // removed clear() when we implemented boxBlur
        // void clear();
        void boxBlur();
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        bool processEvents();
        void close();

};

// END NAMESPACE    
}

// END SCREEN_H_
#endif