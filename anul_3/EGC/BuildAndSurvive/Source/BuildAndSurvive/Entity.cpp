#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Update(float deltaTimeSeconds)
{
}

BOOL Entity::CanDespawn()
{
    return FALSE;
}

char * Entity::GetMeshName()
{
    return meshName;
}

float Entity::GetAngleOX()
{
    return angleOX;
}

float Entity::GetAngleOY()
{
    return angleOY;
}

float Entity::GetAngleOZ()
{
    return angleOZ;
}

void Entity::SetAngleOX(float angle)
{
    angleOX = angle;
}

void Entity::SetAngleOY(float angle)
{
    angleOY = angle;
}

void Entity::SetAngleOZ(float angle)
{
    angleOZ = angle;
}

glm::vec3 Entity::GetPosition()
{
    return position;
}

void Entity::SetPosition(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 Entity::GetScale()
{
    return scale;
}

void Entity::SetScale(glm::vec3 scale)
{
    this->scale = scale;
}

MaterialTypes Entity::GetMaterialType()
{
    return materialType;
}

void Entity::SetMaterialType(MaterialTypes materialType)
{
    this->materialType = materialType;
}

void Entity::SetOrientation(glm::vec3 orientation)
{
    this->orientation = orientation;
}

float Entity::GetRadius()
{
    return radius;
}
