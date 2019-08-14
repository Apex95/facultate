#include "BonusItem.h"

BonusItem::BonusItem()
{
    color = glm::vec3(0.15, 0.53, 0.15);
    bonusLivesColor = glm::vec3(0.8, 0.1, 0.1);
    bonusShieldColor = glm::vec3(0.15, 0.53, 0.15);
    bonusProjectilesColor = glm::vec3(1, 1, 1);

    vertices = vector<VertexFormat>
    {
        VertexFormat(glm::vec3(-0.17, -0.1, 0), color),
        VertexFormat(glm::vec3(0.17f, -0.1, 0), color),
        VertexFormat(glm::vec3(0, 0.2f, 0), color),
    };

    indices = vector<unsigned short>
    {
        0, 1, 2
    };

    name = "bonus_item";

    SetScaleX(1);
    SetScaleY(1);

    collisionRadius = 0.2f;

    speed = 2;
}

BonusItem::~BonusItem()
{
}

unsigned int BonusItem::GetDrawPrimitive()
{
    return GL_TRIANGLES;
}

void BonusItem::Update(float deltaTimeSeconds)
{
    angle += speed * deltaTimeSeconds;

    switch (bonusType)
    {
        case BONUS_LIVES:
            color = bonusLivesColor;
            break;

        case BONUS_SHIELD:
            color = bonusShieldColor;
            break;

        case BONUS_PROJECTILES:
            color = bonusProjectilesColor;
            break;
    }
}

void BonusItem::SetBonusType(BonusType t)
{
    bonusType = t;
}

BonusItem::BonusType BonusItem::GetBonusType()
{
    return bonusType;
}


