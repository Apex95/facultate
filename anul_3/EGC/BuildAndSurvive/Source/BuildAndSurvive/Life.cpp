#include "Entity.h"
#include "Life.h"

Life::Life()
{
    meshName = "Sphere.obj";

    materialType = MaterialTypes::MATERIAL_RED_PLASTIC;

    scale = glm::vec3(0.1f);
}



Life::~Life()
{
}

void Life::SetRotationVector(glm::vec3 rotationVector)
{
    this->rotationVector = rotationVector;
}

glm::vec3 Life::GetRotationVector()
{
    return rotationVector;
}

void Life::SetTranslationVector(glm::vec3 translationVector)
{
    this->translationVector = translationVector;
}

glm::vec3 Life::GetTranslationVector()
{
    return translationVector;
}
