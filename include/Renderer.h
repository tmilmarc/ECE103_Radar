//Header for Renderer.cpp
#pragma once

#include <SDL3/SDL.h>
#include "Radar.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(SDL_Window* window);
    void Draw(Radar& radar);

    void Shutdown();

private:

    SDL_Renderer* renderer;

    int screenWidth;
    int screenHeight;

    float centerX;
    float centerY;

    float pixelsPerCm;
    float radarRadius;

    static constexpr float MAX_RANGE_CM = 300.0f;
    static constexpr float EDGE_MARGIN = 5.0f;

    void DrawArc(float radius);
    void DrawRangeCircles();
    void DrawRadii();
};