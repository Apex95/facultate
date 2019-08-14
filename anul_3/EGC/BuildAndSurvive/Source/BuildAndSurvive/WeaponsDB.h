#pragma once

#include <Core\Engine.h>

typedef struct WeaponProperties
{
    int fireRate;
    int damage;
    int range;
} WeaponProperties;

enum WeaponTypes
{
    WEAPON_PISTOL = 1,
    WEAPON_RIFLE = 2,
    WEAPON_GRENADE_LAUNCHER = 3
};

static std::unordered_map<int, WeaponProperties> weaponsList = 
{
    { WEAPON_PISTOL, WeaponProperties { 4, 5,  15} },
    { WEAPON_RIFLE, WeaponProperties{ 2, 7,  30 } },
    { WEAPON_GRENADE_LAUNCHER, WeaponProperties{ 6, 15,  30 } }
};