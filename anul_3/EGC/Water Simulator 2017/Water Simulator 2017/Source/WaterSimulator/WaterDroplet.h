#pragma once

#include "Entity.h"
#include "Wave.h"

class WaterDroplet : public Entity
{
public:
    WaterDroplet();
    ~WaterDroplet();

    void setPosition(glm::vec3 position) override;
    void applyGravity(float deltaTime) override;

    void setSpawnTime(float spawnTime);
    bool canDespawn() override;

    void applyUpwardForce(float deltaTime);

    void setMaxNofBounces(int nOfBounces);
    int getMaxNofBounces();

    std::list<Wave*> * getWavesList();

    int getNofBounces();

protected:

private:
    float spawnTime = 0;
    float maxSpeed = 0;
    bool goUp = false;

    int nOfBounces = 0;
    int maxNofBounces = 0;

    // fiecare droplet o sa aiba o lista de valuri
    std::list<Wave*> *waves = new std::list<Wave*>;

    bool initHeightSet = false;
    float initHeight;
};