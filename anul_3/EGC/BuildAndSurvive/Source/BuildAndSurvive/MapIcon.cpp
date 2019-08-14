#include "MapIcon.h"

MapIcon::MapIcon()
{
    meshName = "plane50.obj";


    materialType = MaterialTypes::MATERIAL_RED_PLASTIC;

    scale = glm::vec3(0.04f);
}

MapIcon::~MapIcon()
{
}
