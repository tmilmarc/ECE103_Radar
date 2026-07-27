//Owns main loop and initializes SDL
#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application()
{
    window = nullptr;
    renderer = nullptr;
    running = false;
}

Application::~Application()
{
}

bool Application::Initialize()
{

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

        
        
    window = SDL_CreateWindow(
        "ECE103 Radar",
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );

    if (window == nullptr)
    {
        return false;
    }


    renderer = SDL_CreateRenderer(window, nullptr);


    if (renderer == nullptr)
    {
        return false;
    }


    running = true;

    return true;
}

void Application::Run()
{
     SDL_Event event;


    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }       


        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_FRect rectangle;

        rectangle.x = 350;
        rectangle.y = 250;
        rectangle.w = 100;
        rectangle.h = 100;

        SDL_RenderFillRect(renderer, &rectangle);

        SDL_RenderPresent(renderer);
     
    }
}

void Application::Shutdown()
{
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}