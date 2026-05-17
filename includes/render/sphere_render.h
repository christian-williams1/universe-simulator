#pragma once

#include "../config.h"
#include "../render/shader.h"
#include "../simulation/body.h"

class SphereRenderer
{
    public:
        SphereRenderer(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices);
        void draw(const Shader &shader, Body &body, glm::dvec3 worldPos);
        ~SphereRenderer();

    private:
        unsigned int VBO, VAO, EBO;
        int idxCount;
};