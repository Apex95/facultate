#include "SimpleEnemy.h"

SimpleEnemy::SimpleEnemy()
{
    color = glm::vec3(0.05, 0.38, 0.90);
    enhancedColor = glm::vec3(0.84, 0.89, 0.08);
    weakenedColor = glm::vec3(0.9, 0.1, 0.1);



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

    name = "simple_enemy";

    SetScaleX(1);
    SetScaleY(1);

    SetSpeed(2);

    collisionRadius = 0.5f;

    nOfLives = 1;

    isEnhanced = false;
    isWeakened = false;
}

SimpleEnemy::~SimpleEnemy()
{
}

void SimpleEnemy::AdjustAngle(float playerX, float playerY)
{
    angle = atan2(playerY - y, playerX - x) - (float)(M_PI / 2);
}

void SimpleEnemy::Update(float deltaTimeSeconds)
{
    // unghi 0 => adaug doar pe Y;
    // unghi 90 => scad doar de pe X

    x -= speed * sin(angle) * deltaTimeSeconds;
    y += speed * cos(angle) * deltaTimeSeconds;

    if (isWeakened)
    {
        color += glm::vec3((weakenedColor.r - color.r), (weakenedColor.g - color.g), (weakenedColor.b - color.b)) * 4.0f * deltaTimeSeconds;
        scaleX += (weakenedScaleX - scaleX) * 4.0f * deltaTimeSeconds;
        scaleY += (weakenedScaleY - scaleY) * 4.0f * deltaTimeSeconds;
        
        speed = weakenedSpeed;
    }

    if (isEnhanced && !isWeakened)
        color += glm::vec3((enhancedColor.r - color.r), (enhancedColor.g - color.g), (enhancedColor.b - color.b)) * 4.0f * deltaTimeSeconds;

}

void SimpleEnemy::SetLives(int lives)
{

    if (nOfLives > lives && isEnhanced)
        isWeakened = true;

    GameObject::SetLives(lives);
}

unsigned int SimpleEnemy::GetDrawPrimitive()
{
    return GL_TRIANGLES;
}

bool SimpleEnemy::IsWeakened()
{
    return isWeakened;
}

bool SimpleEnemy::IsEnhanced()
{
    return isEnhanced;
}

void SimpleEnemy::Enhance()
{
    isEnhanced = true;

    nOfLives = 2;
    //color = enhancedColor;

}

void SimpleEnemy::SetSpeed(float speed)
{
    GameObject::SetSpeed(speed);
    weakenedSpeed = 2 * speed;
}

void SimpleEnemy::SetScaleX(float scaleX)
{
    GameObject::SetScaleX(scaleX);
    weakenedScaleX = scaleX / 2;

}

void SimpleEnemy::SetScaleY(float scaleY)
{
    GameObject::SetScaleY(scaleY);
    weakenedScaleY = scaleY / 2;
}



