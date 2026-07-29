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

    int choice;
    
    std::cout << "Select data source\n";
    std::cout << "1 - Simulation File\n";
    std::cout << "2 - Serial Port\n";
    std::cout << "> ";
    
    std::cin >> choice;
    
    if(choice == 1)
    {
        dataMode = DataMode::File;
    }
    else
    {
        dataMode = DataMode::Serial;
    }
    
    if(dataMode == DataMode::File)
    {
        if(!fileReader.Open())
        {
            std::cerr << "Could not open radar data file\n";
            return false;
        }
    }
    else
    {
        std::string portName;

        std::cout << "Enter COM port (example COM3): ";
        std::cin >> portName;

        if(!serialPort.Open(portName))
        {
            std::cerr << "Could not open serial port\n";
            return false;
        }
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
        }


        dataTimer += deltaTime;


        if(dataTimer >= dataInterval)
        {
            dataTimer = 0.0f;

            if(dataMode == DataMode::File)
            {
                if(fileReader.Read(angle, distance))
                {
                    radar.Update(angle, distance);
                }
            }
            else
            {
                if(serialPort.Read(angle, distance))
                {
                    radar.Update(angle, distance);
                }
            }
        }


        renderer.Draw(radar);

        SDL_Delay(16);
    }
}

void Application::Shutdown()
{
    renderer.Shutdown();

    serialPort.Close();

    if(window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}