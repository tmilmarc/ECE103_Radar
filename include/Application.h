//Header for Application.cpp
#pragma once

#include "Renderer.h"
#include "FileReader.h"
#include "SerialPort.h"
struct SDL_Window;
enum class DataMode
{
    File,
    Serial
};
enum class AppState
{
    Menu,
    Running
};


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
    FileReader fileReader;
    SerialPort serialPort;
    float dataTimer;
    float dataInterval;
    DataMode dataMode;
    AppState state;
};