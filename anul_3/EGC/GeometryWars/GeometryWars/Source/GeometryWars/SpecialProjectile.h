#pragma once

#include "Projectile.h"

class SpecialProjectile : public Projectile
{
public:
    SpecialProjectile();
    ~SpecialProjectile();
    unsigned int GetDrawPrimitive() override;

    void Update(float deltaTimeSeconds) override;
};