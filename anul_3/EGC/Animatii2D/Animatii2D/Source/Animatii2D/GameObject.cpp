#include "GameObject.h"

GameObject::GameObject()
{
    position = glm::vec2(0, 0);
    scale = glm::vec2(1, 1);
    
}

GameObject::~GameObject()
{
}

std::vector<glm::vec3> GameObject::GetVertices()
{
    return vertices;
}

std::vector<unsigned short> GameObject::GetIndices()
{
    return indices;
}

std::vector<glm::vec2> GameObject::GetTexCoords()
{
    return texCoords;
}

std::vector<glm::vec3> GameObject::GetNormals()
{
    return normals;
}

std::string GameObject::GetMeshName()
{
    return meshName;
}

std::string GameObject::GetTexName()
{
    return texName;
}

glm::vec2 GameObject::GetPosition()
{
    return position;
}

glm::vec2 GameObject::GetScale()
{
    return scale;
}

void GameObject::SetPosition(glm::vec2 position)
{
    this->position = position;
}

void GameObject::Update(float deltaTime)
{
}
