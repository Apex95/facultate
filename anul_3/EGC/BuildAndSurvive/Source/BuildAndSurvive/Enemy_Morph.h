#pragma once

#include <Core/Engine.h>
#include "Entity.h"
#include "Player.h"

class Enemy_Morph : public Entity
{
public:
    Enemy_Morph();
    ~Enemy_Morph();

    void SetWaypoints(std::list<glm::vec3> * waypoints);

    void Update(float deltaTimeSeconds) override;

    int GetHP();
    void SetHP(int HP);

    BOOL CanDespawn() override;
    void ApplyDamage(int damage);

    void SetPlayer(Player * crtPlayer);

private:
    std::list<glm::vec3> *waypoints = NULL;
    std::list<glm::vec3>::iterator waypointsIterator;

    int HP;
    BOOL isDying = false;
    BOOL isDead = false;
    BOOL finishedPath = false;

    Player * crtPlayer;
};