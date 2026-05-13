#pragma once

#include "../config.h"
#include "../render/shader.h"

class SphereRenderer
{
    public:
        SphereRenderer(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices, glm::vec3 color);
        void draw(const Shader &shader, glm::vec3 position, float scale = 0.5f);
        ~SphereRenderer();

    private:
        unsigned int VBO, VAO, EBO;
        glm::vec3 color;
        int idxCount;
};