#include "Water.h"

#include <iostream>

Water::Water()
{
    color = glm::vec3(44/255.0, 144/255.f, 203/255.f);

    kd = 0.5;
    ks = 0.5;
    shininess = 15;

    scale = glm::vec3(1) * 2.0f;
    
    const float SQUARE_SIZE = 0.03;
    const int N_OF_VERTICES = 253;
    
    for (int i = 0; i < N_OF_VERTICES; i++)
    {
        for (int j = 0; j < N_OF_VERTICES; j++)
        { 
            vertices.push_back(VertexFormat(glm::vec3(SQUARE_SIZE * j - (N_OF_VERTICES - 1) * SQUARE_SIZE / 2.0, 0, SQUARE_SIZE * i - (N_OF_VERTICES - 1) * SQUARE_SIZE / 2.0), color));

            if (j < N_OF_VERTICES - 1 && i < N_OF_VERTICES - 1)
            {
                indices.push_back(i * N_OF_VERTICES + j);
                indices.push_back((i + 1) * N_OF_VERTICES + j + 1);
                indices.push_back((i + 1) * N_OF_VERTICES + j);

                indices.push_back(i * N_OF_VERTICES + j);
                indices.push_back(i * N_OF_VERTICES + j + 1);
                indices.push_back((i + 1) * N_OF_VERTICES + j + 1);
                
            }
        }
    }
}

Water::~Water()
{
}

std::vector<VertexFormat> Water::GetVertices()
{
    return vertices;
}

std::vector<unsigned short> Water::GetIndices()
{
    return indices;
}


