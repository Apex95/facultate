#include "GameObject.h"

#include "GameWorld.h"

GameObject::GameObject()
{
    this->color = glm::vec3(0, 0, 0);
}

GameObject::~GameObject()
{
}

vector<VertexFormat> GameObject::GetVertices()
{
    return this->vertices;
}

vector<unsigned short> GameObject::GetIndices()
{
    return this->indices;
}

char * GameObject::GetName()
{
    return this->name;
}

float GameObject::GetX()
{
    return this->x;
}

float GameObject::GetY()
{
    return this->y;
}

void GameObject::SetX(float x)
{
    this->x = x;
}

void GameObject::SetY(float y)
{
    this->y = y;
}

float GameObject::GetAngle()
{
    return this->angle;
}

void GameObject::SetAngle(float angle)
{
    this->angle = angle;
}

float GameObject::GetScaleX()
{
    return this->scaleX;
}

float GameObject::GetScaleY()
{
    return this->scaleY;
}

void GameObject::SetScaleX(float scaleX)
{
    this->scaleX = scaleX;
}

void GameObject::SetScaleY(float scaleY)
{
    this->scaleY = scaleY;
}

float GameObject::GetSpeed()
{
    return this->speed;
}

void GameObject::SetSpeed(float speed)
{
    this->speed = speed;
}

float GameObject::GetCollisionRadius()
{
    return this->collisionRadius;
}

glm::vec3 GameObject::GetColor()
{
    return this->color;
}

int GameObject::GetLives()
{
    return this->nOfLives;
}

void GameObject::SetLives(int lives)
{
    this->nOfLives = lives;
}

unsigned int GameObject::GetDrawPrimitive()
{
    return GL_TRIANGLES; // default
}

void GameObject::Update(float deltaTimeSeconds)
{
}
