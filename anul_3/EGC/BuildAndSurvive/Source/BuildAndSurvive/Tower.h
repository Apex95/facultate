#pragma once

#include <Core/Engine.h>
#include "Entity.h"

class Tower : public Entity
{
public:
    Tower();
    ~Tower();


    void Update(float deltaTimeSeconds) override;
    void SetEnemies(std::list<Entity*> * enemies);
    void SetEntities(std::list<Entity*> * entities);

private:
    std::list<Entity*> *enemies = NULL;
    std::list<Entity*>::iterator enemiesIterator;

    std::list<Entity*> *entities = NULL;

    double fireRate = 1;
    double _lastFiredTime = 0;

    // asta ca sa nu spanwez proiectilele la nivelul solului (consider ca turnul se afla ceva mai sus cand fac calculele)
    glm::vec3 centerOffset = glm::vec3(0, 2, 0);
};