#pragma once

#include "GameObject.h"

class Projectile : public GameObject
{
public:
    Projectile();
    ~Projectile();
    unsigned int GetDrawPrimitive() override;

    void Update(float deltaTimeSeconds) override;

    float GetSpawnTime();
    void SetSpawnTime(float spawnTime);
    float GetTimeToLive();

private:
    float timeToLive;
    float spawnTime;
};