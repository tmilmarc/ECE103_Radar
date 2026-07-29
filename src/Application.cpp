//Owns main loop and initializes SDL
#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application()
    :fileReader("radar_data.txt")
{
    window = nullptr;
    running = false;
    angle = 0.0f;
    distance = 0.0f;
    dataTimer = 0.0f;
    dataInterval = 0.03f;
    state = AppState::Menu;
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "windows");

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

    Uint64 previousTime = SDL_GetTicks();
    

    while(running)
    {
        Uint64 currentTime = SDL_GetTicks();

        float deltaTime =
            (currentTime - previousTime) / 1000.0f;

        previousTime = currentTime;


        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        
        
            if(state == AppState::Menu)
            {
                if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                
                
                    // Simulation button
                    if(renderer.IsSimulationButtonClicked(x,y))
                    {
                        dataMode = DataMode::File;
                        if(!fileReader.Open())
                        {
                            std::cerr << "Could not open radar data file\n";
                        }
                        else
                        {
                            dataTimer = 0.0f;
                            state = AppState::Running;
                        }
                    }
                
                
                    // Serial button
                    else if(renderer.IsSerialButtonClicked(x,y))
                    {
                        dataMode = DataMode::Serial;
                    
                        if(!serialPort.Open("COM3"))
                        {
                            std::cerr << "Serial failed\n";
                        }
                        else
                        {
                            dataTimer = 0.0f;
                            state = AppState::Running;
                        }
                    }
                }
            }
        }

        
        if(state == AppState::Running)
        {
            dataTimer += deltaTime;
        
            if(dataTimer >= dataInterval)
            {
                dataTimer = 0;
            
                if(dataMode == DataMode::File)
                {
                    if(fileReader.Read(angle,distance))
                        radar.Update(angle,distance);
                }
                else
                {
                    if(serialPort.Read(angle,distance))
                        radar.Update(angle,distance);
                }
            }
        
            renderer.Draw(radar);
        }
        else if(state == AppState::Menu)
        {
            renderer.DrawMenu();
        }
        
        

        SDL_Delay(16);
    }
}

void Application::Shutdown()
{
    renderer.Shutdown();

    serialPort.Close();

    fileReader.Close();

    if(window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}