#include "Entity.h"
#include "Tower.h"
#include "Projectile.h"
#include <iostream>


Tower::Tower()
{
    meshName = "Tower.obj";

    materialType = MaterialTypes::MATERIAL_BLACK_IRON;

    scale = glm::vec3(0.007f); 
}



Tower::~Tower()
{
}


void Tower::Update(float deltaTimeSeconds)
{
    for (enemiesIterator = enemies->begin(); enemiesIterator != enemies->end(); enemiesIterator++)
    {
        if (glm::distance((*enemiesIterator)->GetPosition(), position) < 10)
        {
            // inamic prea aproape
            orientation = glm::normalize((*enemiesIterator)->GetPosition() - position - centerOffset * 0.5f);
            angleOY = acos(glm::dot(orientation, forward) / glm::length(orientation * glm::length(forward)));

            if (glm::dot(glm::cross(orientation, forward), glm::vec3(0, 1, 0)) > 0)
                angleOY *= -1;

            if (Engine::GetElapsedTime() - _lastFiredTime > fireRate)
            {
                // spawnez un proiectil
                Projectile* p = new Projectile();
                
                p->SetOrientation(orientation + glm::vec3(0, 0.07f, 0));
                p->SetPosition(position + centerOffset);
                p->SetEnemies(enemies);
                p->SetWeaponType(WeaponTypes::WEAPON_GRENADE_LAUNCHER);

                entities->push_back(p);

                _lastFiredTime = Engine::GetElapsedTime();
            }
        }
    }
}

void Tower::SetEnemies(std::list<Entity*>* enemies)
{
    this->enemies = enemies;
}

void Tower::SetEntities(std::list<Entity*>* entities)
{
    this->entities = entities;
}
