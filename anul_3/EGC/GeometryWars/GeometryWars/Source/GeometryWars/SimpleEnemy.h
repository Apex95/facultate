#pragma once

#include "GameObject.h"

class SimpleEnemy : public GameObject
{
public:
    SimpleEnemy();
    ~SimpleEnemy();

    void AdjustAngle(float playerX, float playerY);
    void Update(float deltaTimeSeconds) override;
    
    void Enhance();
    void SetSpeed(float speed) override;
    void SetScaleX(float scaleX) override;
    void SetScaleY(float scaleY) override;

    void SetLives(int lives) override;
    unsigned int GetDrawPrimitive() override;

    bool IsWeakened();
    bool IsEnhanced();

private:
    bool isEnhanced;
    bool isWeakened;
    glm::vec3 enhancedColor;
    glm::vec3 weakenedColor;
    float weakenedScaleX;
    float weakenedScaleY;
    float weakenedSpeed;
};