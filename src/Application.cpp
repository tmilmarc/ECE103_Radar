//Owns main loop and initializes SDL
#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application()
    
{
    window = nullptr;
    running = false;
    angle = 0.0f;
    distance = 0.0f;
    dataTimer = 0.0f;
    dataInterval = 0.03f;
    state = AppState::MainMenu;
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

             if(event.type == SDL_EVENT_KEY_DOWN)
            {
                if(event.key.key == SDLK_ESCAPE)
                {
                    if(state == AppState::Running)
                    {
                        ReturnToMenu();
                    }
                }
            }
        
        
            if(state == AppState::MainMenu)
            {
                if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    int x = event.button.x;
                    int y = event.button.y;
                
                
                    // Simulation button
                    if(renderer.IsSimulationButtonClicked(x,y))
                    {
                        dataMode = DataMode::File;
                        if(!fileReader.Open("radar_data.txt"))
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
                        availablePorts = serialManager.GetAvailablePorts();
                        
                        if(availablePorts.empty())
                        {
                            std::cerr << "No serial devices found\n";
                        }

                        
                        state = AppState::SerialMenu;
                    }
                }
            }

             if(state == AppState::SerialMenu)
             {
                 if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                 {
                     int x = event.button.x;
                     int y = event.button.y;
                 
                 
                     for(size_t i = 0; i < availablePorts.size(); i++)
                     {
                         if(renderer.IsSerialPortButtonClicked(x,y,i))
                         {
                             if(serialPort.Open(availablePorts[i]))
                             {
                                 dataMode = DataMode::Serial;
                                 dataTimer = 0.0f;
                                 state = AppState::Running;
                             }
                             else
                             {
                                serialPort.Close(); 
                                std::cerr 
                                << "Could not connect to "
                                << availablePorts[i]
                                << "\n";
                             }
                         }
                     }

                     if(renderer.IsRefreshButtonClicked(x,y))
                    {
                        availablePorts = serialManager.GetAvailablePorts();
                    
                        std::cout << "Ports refreshed\n";
                    }



                    // Back button

                    if(renderer.IsBackButtonClicked(x,y))
                    {
                        availablePorts.clear();
                    
                        state = AppState::MainMenu;
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
                    {
                        if(distance >= 0)
                        {
                            radar.Update(angle,distance);
                        }
                    }
                }
                else
                {
                    if(serialPort.Read(angle,distance))
                    {
                        if(distance >= 0)
                        {
                            radar.Update(angle,distance);
                        }
                    }
                }
            }
        
            renderer.Draw(radar);
        }
        else if(state == AppState::MainMenu)
        {
            renderer.DrawMenu();
        }
        else if(state == AppState::SerialMenu)
        {
            renderer.DrawSerialMenu(availablePorts);
        }
        
        

        SDL_Delay(16);
    }
}

void Application::ReturnToMenu()
{
    serialPort.Close();
    fileReader.Close();

    angle = 0.0f;
    distance = 0.0f;

    dataTimer = 0.0f;

    state = AppState::MainMenu;
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