#include "Projectile.h"

Projectile::Projectile()
{
    color = glm::vec3(0.95, 0.73, 0.45);

    vertices = vector<VertexFormat>
    {
        VertexFormat(glm::vec3(-0.1, 0.4, 0), color),
        VertexFormat(glm::vec3(-0.1, -0.4, 0), color),
        VertexFormat(glm::vec3(0.1, -0.4, 0), color),
        VertexFormat(glm::vec3(0.1, 0.4, 0), color),
    };

    indices = vector<unsigned short>
    {
        0, 1, 2,
        2, 3, 0
    };

    name = "projectile";

    SetX(0);
    SetY(0);

    SetLives(1);

    SetScaleX(0.4f);
    SetScaleY(0.4f);

    SetSpeed(12);

    collisionRadius = 0.4f;

    timeToLive = 7;
}

Projectile::~Projectile()
{
}

unsigned int Projectile::GetDrawPrimitive()
{
    return GL_TRIANGLES;
}

void Projectile::Update(float deltaTimeSeconds)
{
    // unghi 0 => adaug doar pe Y;
    // unghi 90 => scad doar de pe X

    x -= speed * sin(angle) * deltaTimeSeconds;
    y += speed * cos(angle) * deltaTimeSeconds;
}

float Projectile::GetSpawnTime()
{
    return spawnTime;
}

void Projectile::SetSpawnTime(float spawnTime)
{
    this->spawnTime = spawnTime;
}

float Projectile::GetTimeToLive()
{
    return timeToLive;
}
