#pragma once

#include <Core/Engine.h>
#include "Entity.h"
#include "Life.h"
#include "WeaponsDB.h"

class Player : public Entity
{
public:
    Player();
    ~Player();

    std::list<Life*> * GetLives();
    float GetLivesFloatingRadius();

    glm::vec3 GetCenterOffset();

    void SetWeaponType(WeaponTypes weaponType);
    WeaponTypes GetWeaponType();

    void SetEntities(std::list<Entity*> *entities);
    void SetEnemies(std::list<Entity*> *enemies);
    
    void FireWeapon();
    
    int GetNOfLives();
    void SetNOfLives(int nOfLives);

    void Update(float deltaTimeSeconds) override;
    BOOL IsDying();

private:
    std::list<Life*> *lives = NULL;
    std::list<Life*>::iterator livesIterator;

    std::list<Entity*> *entities = NULL;
    std::list<Entity*> *enemies = NULL;

    float livesFloatingRadius = 0.5f;

    glm::vec3 centerOffset = glm::vec3(0, 0.7f, 0);

    WeaponTypes weaponType = WeaponTypes::WEAPON_PISTOL;

    double _lastFiredTime;

    int nOfLives = 0;

    BOOL isDying = FALSE;
};