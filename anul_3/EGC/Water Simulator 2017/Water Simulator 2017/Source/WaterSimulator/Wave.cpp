#include "Wave.h"

Wave::Wave()
{
    
}

Wave::~Wave()
{
}

void Wave::setSpawnTime(float time)
{
    spawnTime = time;
    
}

float Wave::getSpawnTime()
{
    return spawnTime;
}

float Wave::getTimeBorder()
{
    return timeBorder;
}

void Wave::setTimeBorder(float timeBorder)
{
    this->timeBorder = timeBorder;
}

bool Wave::canDespawn()
{
    return Engine::GetElapsedTime() - spawnTime > 64 / (*speedCoefficient);
}
