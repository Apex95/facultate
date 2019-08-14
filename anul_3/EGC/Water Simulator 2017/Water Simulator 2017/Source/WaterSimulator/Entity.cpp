#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

int Entity::getShininess()
{
    return shininess;
}

float Entity::getKd()
{
    return kd;
}

float Entity::getKs()
{
    return ks;
}

glm::vec3 Entity::getColor()
{
    return color;
}

Mesh * Entity::getMesh()
{
    return mesh;
}

void Entity::setMesh(Mesh * mesh)
{
    this->mesh = mesh;
}

glm::vec3 Entity::getScale()
{
    return scale;
}

void Entity::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

glm::vec3 Entity::getPosition()
{
    return position;
}

void Entity::setPosition(glm::vec3 position)
{
    this->position = position;
}

void Entity::applyGravity(float deltaTime)
{
}

bool Entity::canDespawn()
{
    return false;
}

void Entity::setSpeedCoefficient(float * speedCoefficient)
{
    this->speedCoefficient = speedCoefficient;
}


