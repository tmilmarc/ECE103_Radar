//Stores scan data
#include "Radar.h"

Radar::Radar()
{
    angle = 0;
    distance = 0;
}

Radar::~Radar()
{
}





void Radar::Update(float newAngle, float newDistance)
{
    angle = newAngle;
    distance = newDistance;
}


float Radar::GetAngle() const
{
    return angle;
}


float Radar::GetDistance() const
{
    return distance;
}