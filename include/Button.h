#pragma once

struct Button
{
    float x;
    float y;
    float width;
    float height;

    bool Contains(float mouseX, float mouseY)
    {
        return mouseX >= x &&
               mouseX <= x + width &&
               mouseY >= y &&
               mouseY <= y + height;
    }
};