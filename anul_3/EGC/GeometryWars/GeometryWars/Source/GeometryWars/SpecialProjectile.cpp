#include "SpecialProjectile.h"

SpecialProjectile::SpecialProjectile()
{
    Projectile::Projectile();

    SetLives(100);
    color = glm::vec3(1, 1, 1);
    speed = 18;

    collisionRadius = 0.5;

    name = "special_projectile";
}

SpecialProjectile::~SpecialProjectile()
{
}

unsigned int SpecialProjectile::GetDrawPrimitive()
{
    return Projectile::GetDrawPrimitive();
}

void SpecialProjectile::Update(float deltaTimeSeconds)
{
    Projectile::Update(deltaTimeSeconds);

    if (scaleY < 3)
        scaleY += speed * deltaTimeSeconds;
    
    
}
