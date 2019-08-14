#pragma once

#include "GameObject.h"

class Shield : public GameObject
{
public:
    Shield();
    ~Shield();

    void Update(float deltaTimeSeconds) override;
    unsigned int GetDrawPrimitive() override;
    void BindTo(GameObject * object);

private:
    GameObject * object = NULL;
};