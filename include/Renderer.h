//Header for Renderer.cpp
#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize();
    void Draw();

    void Shutdown();
};