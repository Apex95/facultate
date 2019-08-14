#include "Entity.h"
#include "Enemy_Morph.h"
#include <iostream>

Enemy_Morph::Enemy_Morph()
{
    meshName = "Morph.obj";

    materialType = MaterialTypes::MATERIAL_BLUE_WATER;

    scale = glm::vec3(0.01f);
    radius = 1.2f;

    HP = 10;
}

Enemy_Morph::~Enemy_Morph()
{
}

void Enemy_Morph::SetWaypoints(std::list<glm::vec3>* waypoints)
{
    this->waypoints = waypoints;

    waypointsIterator = this->waypoints->begin();

    orientation = *(waypointsIterator)-position;
    angleOY = acos(glm::dot(orientation, forward) / glm::length(orientation * glm::length(forward)));


    // cand vectorul orientat spre waypoint se afla "in stanga" vectorului forward, unghiul
    // returnat de arccos (in sens trigonometric) este cel corect si se poate folosi in rotatie
    // dar daca vectorul se afla in partea dreapta a vectorului forward, unghiul intors
    // este calculat in sens orar si prin urmare trebuie inmultit cu (-1) ca sa pot face rotatia in sens trigonometric
    if (glm::dot(glm::cross(orientation, forward), glm::vec3(0, 1, 0)) > 0)
        angleOY *= -1;
}

void Enemy_Morph::Update(float deltaTimeSeconds)
{
    if (isDying)
    {
        // animatie separata

        if (angleOZ < RADIANS(90))
            angleOZ += 1.2f * deltaTimeSeconds;
        
        scale -= glm::vec3(0.01f * deltaTimeSeconds);

        
        if (glm::distance(scale, glm::vec3(0)) < 0.001f)
            isDead = true;

        return;
    }

    if (glm::distance(*(waypointsIterator), position) < 2)
    {
        if (next(waypointsIterator) != waypoints->end())
        {
            waypointsIterator++;

            orientation = glm::normalize(*(waypointsIterator)-position);
            angleOY = acos(glm::dot(orientation, forward) / glm::length(orientation * glm::length(forward)));

            // verific cu regula burghiului daca unghiul este obtinut in sens orar sau trigonometric
            // daca e obtinut in sens orar, va fi orientat in sus deci cos() de unghiul dintre vectorul obtinut din
            // cross-product si OY va fi pozitiv => inversez unghiul
            if (glm::dot(glm::cross(orientation, forward), glm::vec3(0, 1, 0)) > 0)
                angleOY *= -1;
        }
        else
        {
            // am ajuns la final - scad playerului o viata
            crtPlayer->SetNOfLives(crtPlayer->GetNOfLives() - 1);
            finishedPath = true;
            return;
        }
        
    }
    else
    {
        position += orientation * deltaTimeSeconds * 2.0f;
    }
}

int Enemy_Morph::GetHP()
{
    return HP;
}

void Enemy_Morph::SetHP(int HP)
{
    this->HP = HP;
}

BOOL Enemy_Morph::CanDespawn()
{
    return isDead || finishedPath;
}

void Enemy_Morph::ApplyDamage(int damage)
{
    HP -= damage;

    if (HP <= 0)
        isDying = TRUE;
}

void Enemy_Morph::SetPlayer(Player * crtPlayer)
{
    this->crtPlayer = crtPlayer;
}
