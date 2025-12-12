#include <SDL3/SDL.h>
#include <boost/asio.hpp>
#include <webgpu/wgpu.h>
#include "sdl3webgpu.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow("MMO", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Delay(5000);

    SDL_Event event;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
