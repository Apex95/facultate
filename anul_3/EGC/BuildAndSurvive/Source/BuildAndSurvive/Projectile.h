#pragma once

#include <Core/Engine.h>
#include "Entity.h"
#include "WeaponsDB.h"

class Projectile : public Entity
{
public:
    Projectile();
    ~Projectile();

    void Update(float deltaTimeSeconds) override;
    BOOL CanDespawn() override;

    void SetPosition(glm::vec3 position) override;

    void SetWeaponType(WeaponTypes weaponType);
    
    void SetEnemies(std::list<Entity*> *enemies);

private:
    glm::vec3 spawnPosition = glm::vec3(NULL);
    
    // valoare default (suprascrisa atunci cand se seteaza tipul armei)
    float maxDistance = 40;

    // arma din care a fost tras proiectilul
    WeaponTypes weaponType;

    float radius = 0.5f;

    std::list<Entity*> *enemies = NULL;
    std::list<Entity*>::iterator enemiesIterator;

    BOOL enemyHit = FALSE;
};