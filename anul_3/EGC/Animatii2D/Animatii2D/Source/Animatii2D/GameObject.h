#pragma once
#include <Core/Engine.h>
#include <vector>
#include <string>

class GameObject
{
public:
    GameObject();
    ~GameObject();

    std::vector<glm::vec3> GetVertices();
    std::vector<unsigned short> GetIndices();
    std::vector<glm::vec2> GetTexCoords();
    std::vector<glm::vec3> GetNormals();
    
    std::string GetMeshName();
    std::string GetTexName();

    glm::vec2 GetPosition();
    glm::vec2 GetScale();

    void SetPosition(glm::vec2 position);

    virtual void Update(float deltaTime);

protected:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::string meshName;
    std::string texName;

    glm::vec2 position;
    glm::vec2 scale;

private:
    

};