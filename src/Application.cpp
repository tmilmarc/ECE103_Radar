//Owns main loop and initializes SDL
#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application()
{
    window = nullptr;
    running = false;
}

Application::~Application()
{
    Shutdown();
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


   if(!renderer.Initialize(window))
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


        
        renderer.Draw();
     
    }
}

void Application::Shutdown()
{
    renderer.Shutdown();
    SDL_DestroyWindow(window);

    SDL_Quit();
}