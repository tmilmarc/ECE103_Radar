//Stores scan data
#include "Radar.h"

Radar::Radar()
{
    angle = 0;
    distance = 0;
    hasTarget = false;
}

Radar::~Radar()
{
}





void Radar::Update(float newAngle, float newDistance)
{
    angle = newAngle;
    distance = newDistance;
    hasTarget = true;
}


float Radar::GetAngle() const
{
    return angle;
}


float Radar::GetDistance() const
{
    return distance;
}