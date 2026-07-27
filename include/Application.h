//Header for Application.cpp
#pragma once

struct SDL_Window;
struct SDL_Renderer;


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
    SDL_Renderer* renderer;

    bool running;
};