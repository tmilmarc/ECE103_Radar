//Draws everything to the screen
#include "Renderer.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <algorithm>
#include <cstring>

constexpr float PI = 3.14159265358979323846f;

namespace
{
    void DrawCenteredButtonLabel(SDL_Renderer* renderer, const SDL_FRect& rect, const char* label)
    {
        float oldScaleX = 1.0f;
        float oldScaleY = 1.0f;
        SDL_GetRenderScale(renderer, &oldScaleX, &oldScaleY);

        const float targetTextHeight = rect.h * 0.26f;
        const float scale = std::clamp(targetTextHeight / 8.0f, 1.8f, 5.6f);
        const float glyphSize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * scale;
        const float textWidth = static_cast<float>(std::strlen(label)) * glyphSize * 0.8f;
        const float x = rect.x + (rect.w - textWidth) / 2.0f;
        const float y = rect.y + (rect.h - glyphSize) / 2.0f;

        SDL_SetRenderScale(renderer, scale, scale);

        const float drawX = x / scale;
        const float drawY = y / scale;

        for(int iy = -1; iy <= 1; ++iy)
        {
            for(int ix = -1; ix <= 1; ++ix)
            {
                if(ix == 0 && iy == 0)
                {
                    continue;
                }

                SDL_RenderDebugText(renderer, drawX + ix * 0.1f, drawY + iy * 0.1f, label);
            }
        }

        SDL_RenderDebugText(renderer, drawX, drawY, label);
        SDL_SetRenderScale(renderer, oldScaleX, oldScaleY);
    }
}

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
    SDL_GetRenderOutputSize(renderer, &screenWidth, &screenHeight);

    SDL_SetRenderDrawColor(
        renderer,
        20,
        20,
        20,
        255
    );

    SDL_RenderClear(renderer);

    const float buttonWidth = std::clamp(static_cast<float>(screenWidth) * 0.72f, 500.0f, 820.0f);
    const float buttonHeight = std::clamp(static_cast<float>(screenHeight) * 0.18f, 110.0f, 160.0f);
    const float gap = std::clamp(static_cast<float>(screenHeight) * 0.05f, 35.0f, 90.0f);
    const float buttonX = (screenWidth - buttonWidth) / 2.0f;
    const float startY = (screenHeight - (buttonHeight * 2.0f + gap)) / 2.0f;

    simulationButton.x = buttonX;
    simulationButton.y = startY;
    simulationButton.width = buttonWidth;
    simulationButton.height = buttonHeight;

    SDL_SetRenderDrawColor(
        renderer,
        0,
        120,
        0,
        255
    );

    SDL_FRect simulationRect = { simulationButton.x, simulationButton.y, simulationButton.width, simulationButton.height };
    SDL_RenderFillRect(
        renderer,
        &simulationRect
    );

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );
    DrawCenteredButtonLabel(renderer, simulationRect, "Simulation");

    serialButton.x = buttonX;
    serialButton.y = startY + buttonHeight + gap;
    serialButton.width = buttonWidth;
    serialButton.height = buttonHeight;

    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        150,
        255
    );

    SDL_FRect serialRect = { serialButton.x, serialButton.y, serialButton.width, serialButton.height };
    SDL_RenderFillRect(
        renderer,
        &serialRect
    );

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );
    DrawCenteredButtonLabel(renderer, serialRect, "Serial");

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
    SDL_GetRenderOutputSize(renderer, &screenWidth, &screenHeight);

    SDL_SetRenderDrawColor(
        renderer,
        30,
        30,
        30,
        255
    );

    SDL_RenderClear(renderer);

    const float buttonWidth = std::clamp(static_cast<float>(screenWidth) * 0.72f, 500.0f, 900.0f);
    const float buttonX = (screenWidth - buttonWidth) / 2.0f;
    const float portButtonWidth = buttonWidth;
    const float portButtonHeight = std::clamp(static_cast<float>(screenHeight) * 0.09f, 60.0f, 92.0f);
    const float rowGap = 18.0f;
    const float topY = std::max(90.0f, (screenHeight - (std::max(1, static_cast<int>(ports.size())) * (portButtonHeight + rowGap) + 50.0f)) / 2.0f);

    if(ports.empty())
    {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_FRect box = { buttonX, topY, portButtonWidth, portButtonHeight * 1.8f };
        SDL_RenderFillRect(renderer, &box);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        DrawCenteredButtonLabel(renderer, box, "No devices found");
    }
    else
    {
        for(int i = 0; i < ports.size(); i++)
        {
            SDL_FRect button;

            button.x = buttonX;
            button.y = topY + i * (portButtonHeight + rowGap);
            button.w = portButtonWidth;
            button.h = portButtonHeight;

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

            SDL_SetRenderDrawColor(
                renderer,
                255,
                255,
                255,
                255
            );
            DrawCenteredButtonLabel(renderer, button, ports[i].c_str());
        }
    }

    SDL_FRect refreshButton;

    refreshButton.x = buttonX;
    refreshButton.y = static_cast<float>(screenHeight) - 110.0f;
    refreshButton.w = buttonWidth * 0.45f;
    refreshButton.h = 72.0f;

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

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );
    DrawCenteredButtonLabel(renderer, refreshButton, "Refresh");

    SDL_FRect backButton;

    backButton.x = buttonX + buttonWidth * 0.55f;
    backButton.y = static_cast<float>(screenHeight) - 110.0f;
    backButton.w = buttonWidth * 0.45f;
    backButton.h = 72.0f;

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

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );
    DrawCenteredButtonLabel(renderer, backButton, "Back");

    SDL_RenderPresent(renderer);
}

bool Renderer::IsSerialPortButtonClicked(
    int x,
    int y,
    int index
)
{
    SDL_GetRenderOutputSize(renderer, &screenWidth, &screenHeight);

    const float buttonWidth = std::clamp(static_cast<float>(screenWidth) * 0.72f, 500.0f, 900.0f);
    const float buttonX = (screenWidth - buttonWidth) / 2.0f;
    const float portButtonHeight = std::clamp(static_cast<float>(screenHeight) * 0.09f, 60.0f, 92.0f);
    const float rowGap = 18.0f;
    const float topY = std::max(90.0f, (screenHeight - ((std::max(1, index + 1)) * (portButtonHeight + rowGap) + 50.0f)) / 2.0f);
    const float buttonY = topY + index * (portButtonHeight + rowGap);

    return x > buttonX &&
           x < buttonX + buttonWidth &&
           y > buttonY &&
           y < buttonY + portButtonHeight;
}

bool Renderer::IsRefreshButtonClicked(int x, int y)
{
    SDL_GetRenderOutputSize(renderer, &screenWidth, &screenHeight);

    const float buttonWidth = std::clamp(static_cast<float>(screenWidth) * 0.72f, 500.0f, 900.0f);
    const float buttonX = (screenWidth - buttonWidth) / 2.0f;
    const float refreshX = buttonX;
    const float refreshY = static_cast<float>(screenHeight) - 110.0f;
    const float refreshW = buttonWidth * 0.45f;

    return x > refreshX &&
           x < refreshX + refreshW &&
           y > refreshY &&
           y < refreshY + 50.0f;
}


bool Renderer::IsBackButtonClicked(int x, int y)
{
    SDL_GetRenderOutputSize(renderer, &screenWidth, &screenHeight);

    const float buttonWidth = std::clamp(static_cast<float>(screenWidth) * 0.72f, 500.0f, 900.0f);
    const float buttonX = (screenWidth - buttonWidth) / 2.0f;
    const float backX = buttonX + buttonWidth * 0.55f;
    const float backY = static_cast<float>(screenHeight) - 110.0f;
    const float backW = buttonWidth * 0.45f;

    return x > backX &&
           x < backX + backW &&
           y > backY &&
           y < backY + 50.0f;
}