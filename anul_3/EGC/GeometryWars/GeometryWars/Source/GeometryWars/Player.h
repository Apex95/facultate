#pragma once

#include "GameObject.h"
#include "GameWorld.h"

class Player : public GameObject
{
public: 
    Player();
    ~Player();

    unsigned int GetDrawPrimitive() override;
    bool HasActiveShield();
    void ShieldActive(bool status);
private:
    bool hasActiveShield;
};