#include "Entity.h"
#include "Player.h"
#include "Life.h"
#include "Projectile.h"

#include <iostream>

Player::Player()
{
    meshName = "Spectre.obj";

    materialType = MaterialTypes::MATERIAL_MAUVE_IRON;

    scale = glm::vec3(0.01f);


    lives = new std::list<Life*>;

    Life * l = NULL;

    // hardcodate 3 vieti
    l = new Life();
    l->SetRotationVector(glm::vec3(1, 0, 0));
    l->SetTranslationVector(glm::vec3(0, 1, 0));
    l->SetPosition(position + l->GetTranslationVector() * livesFloatingRadius + centerOffset);
    lives->push_back(l);

    l = new Life();
    l->SetRotationVector(glm::vec3(0, 1, 0));
    l->SetTranslationVector(glm::vec3(0, 0, 1));
    l->SetPosition(position + l->GetTranslationVector() * livesFloatingRadius + centerOffset);
    lives->push_back(l);

    l = new Life();
    l->SetRotationVector(glm::vec3(0, 0, 1));
    l->SetTranslationVector(glm::vec3(1, 0, 0));
    l->SetPosition(position + l->GetTranslationVector() * livesFloatingRadius + centerOffset);
    lives->push_back(l);

    nOfLives = (int)lives->size();
}

Player::~Player()
{
}

std::list<Life*> * Player::GetLives()
{
    return lives;
}

float Player::GetLivesFloatingRadius()
{
    return livesFloatingRadius;
}

glm::vec3 Player::GetCenterOffset()
{
    return centerOffset;
}

void Player::SetWeaponType(WeaponTypes weaponType)
{
    this->weaponType = weaponType;
    _lastFiredTime = 0; // am schimbat arma, primul proiectil poate fi lansat instant
}

WeaponTypes Player::GetWeaponType()
{
    return weaponType;
}

void Player::SetEntities(std::list<Entity*>* entities)
{
    this->entities = entities;
}

void Player::SetEnemies(std::list<Entity*>* enemies)
{
    this->enemies = enemies;
}

void Player::FireWeapon()
{
    if (Engine::GetElapsedTime() - _lastFiredTime < weaponsList[weaponType].fireRate)
        return;

    _lastFiredTime = Engine::GetElapsedTime() - weaponsList[weaponType].fireRate/1.1;

    Projectile * p = new Projectile();
    p->SetOrientation(orientation + centerOffset * 0.5f); // ridic putin vectorul de orientare
    p->SetPosition(position + centerOffset);
    p->SetEnemies(enemies);
    p->SetWeaponType(weaponType);

    entities->push_back(p);
}

int Player::GetNOfLives()
{
    return nOfLives;
}

void Player::SetNOfLives(int nOfLives)
{
    this->nOfLives = nOfLives;

    if (nOfLives == 0)
        isDying = TRUE;
}

void Player::Update(float deltaTimeSeconds)
{
    if (isDying)
    {
        if (angleOZ < RADIANS(90))
            angleOZ += 1.2f * deltaTimeSeconds;
    }
}

BOOL Player::IsDying()
{
    return isDying;
}








