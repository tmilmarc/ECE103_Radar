//Header for Radar.cpp
#pragma once

class Radar
{
public:
    Radar();
    ~Radar();

    void Update(float angle, float distance);

    float GetAngle() const;
    float GetDistance() const;

private:
    float angle;
    float distance;
};