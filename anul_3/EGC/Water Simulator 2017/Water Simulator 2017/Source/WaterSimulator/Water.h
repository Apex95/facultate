#pragma once

#include <Core/Engine.h>
#include "Entity.h"

class Water : public Entity
{
public:
    Water();
    ~Water();

    std::vector<VertexFormat> GetVertices();
    std::vector<unsigned short> GetIndices();


    const float WAVELENGTH = 0.8;
    const float SPEED = 0.5f;
    const float AMPLITUDE = 0.3;
    const float FREQUENCY = 2 * M_PI / WAVELENGTH;
    const float _PHASE = SPEED * FREQUENCY;
    float PHASE = _PHASE;

protected:

private:
    std::vector<VertexFormat> vertices;
    std::vector<unsigned short> indices;
};