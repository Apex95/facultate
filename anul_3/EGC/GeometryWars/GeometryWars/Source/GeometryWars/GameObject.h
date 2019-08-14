#pragma once

#include <vector>
#include <Core/Engine.h>

using namespace std;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    vector<VertexFormat> GetVertices();
    vector<unsigned short> GetIndices();


    // -- getters & setters
    char * GetName();
    float GetX();
    float GetY();
    virtual void SetX(float x);
    virtual void SetY(float y);
    float GetAngle();
    virtual void SetAngle(float angle);
    float GetScaleX();
    float GetScaleY();
    
    virtual void SetScaleX(float scaleX);
    virtual void SetScaleY(float scaleY);

    float GetSpeed();
    virtual void SetSpeed(float speed);

    float GetCollisionRadius();

    glm::vec3 GetColor();

    int GetLives();
    virtual void SetLives(int lives);

    virtual unsigned int GetDrawPrimitive();
    virtual void Update(float deltaTimeSeconds);
    
protected:
    vector<VertexFormat> vertices;
    vector<unsigned short> indices;
    char * name;
    float x;
    float y;
    float angle = 0;
    float scaleX = 1;
    float scaleY = 1;

    float speed = 0;

    float collisionRadius = 0;

    glm::vec3 color;

    int nOfLives;


};