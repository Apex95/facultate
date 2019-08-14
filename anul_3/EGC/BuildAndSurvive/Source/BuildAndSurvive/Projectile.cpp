#include "Entity.h"
#include "Projectile.h"
#include "Enemy_Morph.h"

#include <iostream>

Projectile::Projectile()
{
    meshName = "Sphere.obj";


    materialType = MaterialTypes::MATERIAL_ORANGE_PLASMA;

    scale = glm::vec3(0.3f);

    radius = 0.3f * 1;
}



Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTimeSeconds)
{
    position += orientation * deltaTimeSeconds * 25.0f;

    // adaug efectul de bolta (fac grenada sa cada spre podea)
    if (weaponType == WeaponTypes::WEAPON_GRENADE_LAUNCHER)
        orientation -= glm::vec3(0, 0.01f, 0);

    // verific coliziunea cu inamicul
    for (enemiesIterator = enemies->begin(); enemiesIterator != enemies->end(); enemiesIterator++)
    {
        if (glm::distance((*enemiesIterator)->GetPosition(), position) < (*enemiesIterator)->GetRadius() + radius)
        {
            enemyHit = TRUE;
            ((Enemy_Morph*)(*enemiesIterator))->ApplyDamage(weaponsList[weaponType].damage);
        }
    }
}

BOOL Projectile::CanDespawn()
{
    return glm::distance(position, spawnPosition) > weaponsList[weaponType].range || enemyHit;
}

void Projectile::SetPosition(glm::vec3 position)
{
    Entity::SetPosition(position);

    // daca nu e setata pozitia de spawn, preiau
    // de aici locatia curenta
    if (spawnPosition == glm::vec3(NULL))
        spawnPosition = glm::vec3(position);
}

void Projectile::SetWeaponType(WeaponTypes weaponType)
{
    this->weaponType = weaponType;
}

void Projectile::SetEnemies(std::list<Entity*>* enemies)
{
    this->enemies = enemies;
}
