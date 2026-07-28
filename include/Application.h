//Header for Application.cpp
#pragma once

#include "Renderer.h"
struct SDL_Window;


class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

    private:

    SDL_Window* window;
    Renderer renderer;
    Radar radar;

    bool running;

    float angle;
    float distance;
    bool increasing;
};