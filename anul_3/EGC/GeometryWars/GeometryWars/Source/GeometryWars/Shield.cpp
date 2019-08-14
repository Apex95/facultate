#include "Shield.h"
#include "Player.h"

Shield::Shield()
{
    color = glm::vec3(0.15, 0.53, 0.15);

    float shieldRadius = 1;

    for (int i = 0; i < 360; i++)
        vertices.push_back(VertexFormat(glm::vec3(shieldRadius * cos(RADIANS(i)), shieldRadius * sin(RADIANS(i)), 0), color));
       
    int counter = 0;

    for (int i = 0; i < 360; i++)
    {
        if (counter < 20)
        {
            indices.push_back(i);
            counter++;
        }
        else
        {
            i += counter;
            counter = 0;
        }
    }
    
   

    name = "shield";


    SetScaleX(1);
    SetScaleY(1);

    SetSpeed(3);

    collisionRadius = shieldRadius;
}

Shield::~Shield()
{
}

void Shield::Update(float deltaTimeSeconds)
{
    // scut spawnat, dar nu are un obiect pe care sa-l apere
    if (object == NULL)
        return;   

    x = object->GetX();
    y = object->GetY();

    angle += speed * deltaTimeSeconds;
}

unsigned int Shield::GetDrawPrimitive()
{
    return GL_LINES;
}

void Shield::BindTo(GameObject * object)
{
    this->object = object;
}
