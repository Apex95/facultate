#pragma once

#include <Core/Engine.h>
#include "MaterialsDB.h"

class Entity
{
public:
    Entity();
    ~Entity();

protected:
    char * meshName;
    float angleOX = 0, angleOY = 0, angleOZ = 0;
    glm::vec3 position;
    glm::vec3 scale;

    MaterialTypes materialType;

    glm::vec3 forward = glm::vec3(0, 0, -1);
    glm::vec3 orientation;

    float radius;

public:
    virtual void Update(float deltaTimeSeconds);
    virtual BOOL CanDespawn();

    // -- getters & setters
    char * GetMeshName();

    float GetAngleOX();
    float GetAngleOY();
    float GetAngleOZ();

    void SetAngleOX(float angle);
    void SetAngleOY(float angle);
    void SetAngleOZ(float angle);

    glm::vec3 GetPosition();
    virtual void SetPosition(glm::vec3 position);

    glm::vec3 GetScale();
    void SetScale(glm::vec3 scale);

    MaterialTypes GetMaterialType();
    void SetMaterialType(MaterialTypes materialType);

    void SetOrientation(glm::vec3 orientation);

    float GetRadius();
};