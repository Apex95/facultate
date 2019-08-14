#pragma once

#include "GameObject.h"
#include "GameWorld.h"

class BonusItem : public GameObject
{
public:
    BonusItem();
    ~BonusItem();

    unsigned int GetDrawPrimitive() override;
    void Update(float deltaTimeSeconds) override;

    enum BonusType { BONUS_LIVES, BONUS_PROJECTILES, BONUS_SHIELD};
    
    void SetBonusType(BonusType t);
    BonusType GetBonusType();

private:
    glm::vec3 bonusLivesColor, bonusProjectilesColor, bonusShieldColor;
    BonusType bonusType;
    
};