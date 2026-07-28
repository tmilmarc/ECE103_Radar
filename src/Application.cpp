//Owns main loop and initializes SDL
#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application()
{
    window = nullptr;
    running = false;
    angle = 0.0f;
    distance = 200.0f;
    increasing = true;
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


        
        if (increasing)
        {
            angle += 1.0f;
        
            if (angle >= 180.0f)
            {
                angle = 180.0f;
                increasing = false;
            }
        }
        else
        {
            angle -= 1.0f;
        
            if (angle <= 0.0f)
            {
                angle = 0.0f;
                increasing = true;
            }
        }
        
        radar.Update(angle, distance);
        renderer.Draw(radar);
     
    }
}

void Application::Shutdown()
{
    renderer.Shutdown();
    SDL_DestroyWindow(window);

    SDL_Quit();
}