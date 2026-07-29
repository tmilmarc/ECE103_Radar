//Header for Application.cpp
#pragma once

#include "Renderer.h"
#include "FileReader.h"
#include "SerialPort.h"
#include "SerialManager.h"
#include <vector>
struct SDL_Window;
enum class DataMode
{
    File,
    Serial
};
enum class AppState
{
    MainMenu,
    FileMenu,
    SerialMenu,
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

    void ReturnToMenu();

    SerialManager serialManager;

    std::vector<std::string> availablePorts;
};