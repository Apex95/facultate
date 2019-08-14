#include "Player.h"

using namespace std;

Player::Player()
{
    color = glm::vec3(0.18, 0.31, 0.33);
    
    vertices = vector<VertexFormat>
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(0.5f, 0.5f, 0), color),
        VertexFormat(glm::vec3(0.5f, -0.5f, 0), color),
        VertexFormat(glm::vec3(-0.5f, 0.5f, 0), color),
        VertexFormat(glm::vec3(-0.5f, -0.5f, 0), color),
    };

    indices = vector<unsigned short>
    {
        0, 2, 1,
        0, 3, 4
    };

    name = "player";

    // hardcodate....meh
    SetX(5);
    SetY(5);

    SetScaleX(1);
    SetScaleY(1);

    SetSpeed(5);

    SetLives(3);

    collisionRadius = 0.5f;
}

Player::~Player()
{
}

unsigned int Player::GetDrawPrimitive()
{
    return GL_TRIANGLES;
}

bool Player::HasActiveShield()
{
    return hasActiveShield;
}

void Player::ShieldActive(bool status)
{
    hasActiveShield = status;
}

