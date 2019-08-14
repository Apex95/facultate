#pragma once

#include <Core\Engine.h>

typedef struct MaterialProperties
{
    glm::vec3 color;
    int shininess;
    float kd;
    float ks;
} MaterialProperties;

enum MaterialTypes
{
    MATERIAL_BLACK_IRON = 1,
    MATERIAL_GREEN_FLESH = 2,
    MATERIAL_RED_PLASTIC = 3,
    MATERIAL_MAUVE_IRON = 4,
    MATERIAL_BLUE_WATER = 5,
    MATERIAL_ORANGE_PLASMA = 6,
};

static std::unordered_map<int, MaterialProperties> materialsList = 
{
    { MATERIAL_BLACK_IRON, MaterialProperties {glm::vec3(40/255.0, 40/255.0, 40/255.0), 30, 0.9f, 0.8f} },
    { MATERIAL_GREEN_FLESH, MaterialProperties{ glm::vec3(34 / 255.0, 190 / 255.0, 23 / 255.0), 10, 0.5f, 0.4f } },
    { MATERIAL_RED_PLASTIC, MaterialProperties{ glm::vec3(200 / 255.0, 15 / 255.0, 15 / 255.0), 20, 0.7f, 0.6f } },
    { MATERIAL_MAUVE_IRON, MaterialProperties{ glm::vec3(83 / 255.0, 42 / 255.0, 105 / 255.0), 20, 0.7f, 0.6f } },
    { MATERIAL_BLUE_WATER, MaterialProperties{ glm::vec3(2 / 255.0, 158 / 255.0, 246 / 255.0), 25, 0.75f, 0.7f } },
    { MATERIAL_ORANGE_PLASMA, MaterialProperties{ glm::vec3(220 / 255.0, 75/ 255.0, 13 / 255.0), 45, 2.0f, 1.5f } }
};