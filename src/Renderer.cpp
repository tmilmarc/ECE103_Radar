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

void Renderer::Draw()
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


    // Fake radar data
    float angle = 180.0f;
    float distanceCm = 200.0f;

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

void Renderer::Shutdown()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
}