#pragma once

#include "Entity.h"

class Wave : public Entity
{
public:
    Wave();
    ~Wave();

    void setSpawnTime(float time);
    float getSpawnTime();

    

    float getTimeBorder();
    void setTimeBorder(float timeBorder);

    bool canDespawn() override;

protected:

private:
    float timeBorder = 0;
    float spawnTime;
};