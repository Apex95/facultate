#pragma once

#include <Core/Engine.h>

class Entity
{
public:
    Entity();
    ~Entity();

    // -- getters & setters
    virtual int getShininess();
    virtual float getKd();
    virtual float getKs();
    virtual glm::vec3 getColor();

    virtual Mesh * getMesh();
    virtual void setMesh(Mesh * mesh);

    virtual glm::vec3 getScale();
    virtual void setScale(glm::vec3 scale);

    virtual glm::vec3 getPosition();
    virtual void setPosition(glm::vec3 position);

    virtual void applyGravity(float deltaTime);
    virtual bool canDespawn();

    virtual void setSpeedCoefficient(float * speedCoefficient);

protected:
    int shininess = 25;
    float kd = 0.75f;
    float ks = 0.7f;
    glm::vec3 color = glm::vec3(2 / 255.0, 158 / 255.0, 246 / 255.0);

    Mesh * mesh;

    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::vec3 position = glm::vec3(0, 0, 0);

    float mass = 0;

    float * speedCoefficient = NULL;

private:
    
};