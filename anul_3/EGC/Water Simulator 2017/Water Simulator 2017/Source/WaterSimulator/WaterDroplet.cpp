#include "WaterDroplet.h"

WaterDroplet::WaterDroplet()
{
    scale = glm::vec3(1, 1, 1);

    mass = 0.000008;

    
    position = glm::vec3(0, 100, 0);
    
}

float radius = 0.5 * 0.5 * 0.001;

WaterDroplet::~WaterDroplet()
{
}

void WaterDroplet::setPosition(glm::vec3 position)
{
    Entity::setPosition(position);

    if (!initHeightSet)
    {
        initHeightSet = true;
        initHeight = MIN(position.y, 4);
    }
}

void WaterDroplet::applyGravity(float deltaTime)
{
    Entity::applyGravity(deltaTime);


    if (goUp)
    {
        applyUpwardForce(deltaTime);

        if (position.y > initHeight + nOfBounces)
        {
            goUp = false;
            nOfBounces++;
            spawnTime = Engine::GetElapsedTime();
        }
    }
    else
    {
        maxSpeed = sqrt(2 * mass * 9.8 / (1.204 * 0.47 * (radius * radius * M_PI)));
        position -= glm::vec3(0, MIN(maxSpeed, 9.8 * (Engine::GetElapsedTime() - spawnTime)) * deltaTime, 0);
    }
    
    if (position.y < 0.1)
    {
        position.y = 0;
        
        mass /= 2.0;
        scale /= 2.0;
        
        if (nOfBounces < maxNofBounces)
        {
            goUp = true;
            spawnTime = Engine::GetElapsedTime();
        }
    }
}

void WaterDroplet::setSpawnTime(float spawnTime)
{
    this->spawnTime = spawnTime;

    scale = glm::vec3(1, 1, 1);
    mass = 0.000008;

    nOfBounces = 0;
}

bool WaterDroplet::canDespawn()
{
    return nOfBounces >= maxNofBounces && waves->size() == 0;
}

void WaterDroplet::applyUpwardForce(float deltaTime)
{
    // conservarea energiei (Ec = Ep(m/2))
    float speed = sqrt(9.8 * ((initHeight - position.y) + nOfBounces));
    position += glm::vec3(0, speed, 0) * deltaTime;

}

void WaterDroplet::setMaxNofBounces(int nOfBounces)
{
    maxNofBounces = nOfBounces;
}

int WaterDroplet::getMaxNofBounces()
{
    return maxNofBounces;
}

std::list<Wave*> * WaterDroplet::getWavesList()
{
    return waves;
}

int WaterDroplet::getNofBounces()
{
    return nOfBounces;
}
