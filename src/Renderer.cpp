//Draws everything to the screen
#include "Renderer.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159265358979323846f;

Renderer::Renderer()
{
    renderer = nullptr;

    screenWidth = 800;
    screenHeight = 600;

    centerX = 400;
    centerY = 600;

    simulationButton.x = 250;
    simulationButton.y = 200;
    simulationButton.width = 300;
    simulationButton.height = 80;


    serialButton.x = 250;
    serialButton.y = 320;
    serialButton.width = 300;
    serialButton.height = 80;
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(SDL_Window* window)
{
    renderer = SDL_CreateRenderer(window, nullptr);

    if(renderer == nullptr)
    {
        return false;
    }

    return true;
}

void Renderer::Draw(Radar& radar)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);


    SDL_GetRenderOutputSize(
        renderer,
        &screenWidth,
        &screenHeight
    );


    // Radar origin: bottom center of screen
    centerX = screenWidth / 2.0f;
    centerY = screenHeight - EDGE_MARGIN;
    radarRadius = screenWidth / 2.0f;

    radarRadius = std::min(screenWidth / 2.0f, (float)screenHeight) - EDGE_MARGIN;
    pixelsPerCm = radarRadius / MAX_RANGE_CM;
   
    DrawRangeCircles();
    DrawRadii();


    
    float angle = radar.GetAngle();
    float distanceCm = radar.GetDistance();

    float distancePixels = distanceCm * pixelsPerCm;


    // Convert degrees to radians
    float radians = angle * PI / 180.0f;


    // Convert radar coordinates to screen coordinates
    float targetX =
        centerX + distancePixels * cos(radians);


    float targetY =
        centerY - distancePixels* sin(radians);



    // Draw target
    SDL_SetRenderDrawColor(
        renderer,
        255,
        0,
        0,
        255
    );


    SDL_FRect target;

    target.x = targetX - 10;
    target.y = targetY - 10;
    target.w = 20;
    target.h = 20;


    SDL_RenderFillRect(
        renderer,
        &target
    );


    SDL_RenderPresent(renderer);
}

void Renderer::DrawArc(float radius)
{
    

    float previousX = centerX + radius;
    float previousY = centerY;


    SDL_SetRenderDrawColor(
        renderer,
        0,
        150,
        0,
        255
    );


    for(int angle = 0; angle <= 180; angle++)
    {
        float radians = angle * PI / 180.0f;


        float x =
            centerX + radius * cos(radians);


        float y =
            centerY - radius * sin(radians);


        SDL_RenderLine(
            renderer,
            previousX,
            previousY,
            x,
            y
        );


        previousX = x;
        previousY = y;
    }
}

void Renderer::DrawRangeCircles()
{
    for(int i = 1; i <= 4; i++)
    {
        float radius = radarRadius * (i / 4.0f);

        DrawArc(radius);
    }
}

void Renderer::DrawRadii()
{
    SDL_SetRenderDrawColor(
        renderer,
        0,
        150,
        0,
        255
    );


    for(int i = 0; i < 9; i++)
    {
        float angle = i * (180.0f / 8.0f);

        float radians = angle * PI / 180.0f;


        float x =
            centerX + radarRadius * cos(radians);


        float y =
            centerY - radarRadius * sin(radians);


        SDL_RenderLine(
            renderer,
            centerX,
            centerY,
            x,
            y
        );
    }
}

void Renderer::DrawMenu()
{
    SDL_SetRenderDrawColor(
        renderer,
        20,
        20,
        20,
        255
    );

    SDL_RenderClear(renderer);


    // Simulation button
    SDL_FRect simulationButton;

    simulationButton.x = 250;
    simulationButton.y = 200;
    simulationButton.w = 300;
    simulationButton.h = 80;


    SDL_SetRenderDrawColor(
        renderer,
        0,
        120,
        0,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &simulationButton
    );


    // Serial button
    SDL_FRect serialButton;

    serialButton.x = 250;
    serialButton.y = 320;
    serialButton.w = 300;
    serialButton.h = 80;


    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        150,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &serialButton
    );


    SDL_RenderPresent(renderer);
}

void Renderer::Shutdown()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
}

bool Renderer::IsSimulationButtonClicked(float x, float y)
{
    return simulationButton.Contains(x,y);
}


bool Renderer::IsSerialButtonClicked(float x, float y)
{
    return serialButton.Contains(x,y);
}

void Renderer::DrawSerialMenu(
    const std::vector<std::string>& ports
)
{
    SDL_SetRenderDrawColor(
        renderer,
        30,
        30,
        30,
        255
    );

    SDL_RenderClear(renderer);


    // Draw each COM port as a button

    for(int i = 0; i < ports.size(); i++)
    {
        SDL_FRect button;

        button.x = 250;
        button.y = 150 + i * 80;
        button.w = 300;
        button.h = 50;


        SDL_SetRenderDrawColor(
            renderer,
            0,
            100,
            200,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &button
        );
    }

    // Refresh button

    SDL_FRect refreshButton;

    refreshButton.x = 250;
    refreshButton.y = 500;
    refreshButton.w = 140;
    refreshButton.h = 50;


    SDL_SetRenderDrawColor(
        renderer,
        0,
        150,
        0,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &refreshButton
    );



    // Back button

    SDL_FRect backButton;

    backButton.x = 410;
    backButton.y = 500;
    backButton.w = 140;
    backButton.h = 50;


    SDL_SetRenderDrawColor(
        renderer,
        150,
        0,
        0,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &backButton
    );


    SDL_RenderPresent(renderer);
}

bool Renderer::IsSerialPortButtonClicked(
    int x,
    int y,
    int index
)
{
    int buttonY = 150 + index * 80;


    return
        x > 250 &&
        x < 550 &&
        y > buttonY &&
        y < buttonY + 50;
}

bool Renderer::IsRefreshButtonClicked(int x, int y)
{
    return
        x > 250 &&
        x < 390 &&
        y > 500 &&
        y < 550;
}


bool Renderer::IsBackButtonClicked(int x, int y)
{
    return
        x > 410 &&
        x < 550 &&
        y > 500 &&
        y < 550;
}