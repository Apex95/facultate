#pragma once

#include <Core/Engine.h>
#include "Entity.h"

class Life : public Entity
{
public:
    Life();
    ~Life();

    void SetRotationVector(glm::vec3 rotationVector);
    glm::vec3 GetRotationVector();

    void SetTranslationVector(glm::vec3 translationVector);
    glm::vec3 GetTranslationVector();

private:
    glm::vec3 rotationVector;
    glm::vec3 translationVector;
};