#pragma once

#include "../config.h"
#include "../render/shader.h"
#include "../simulation/body.h"

class SphereRenderer
{
    public:
        SphereRenderer(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices, Shader &shader);
        void draw(Body &body, glm::dvec3 worldPos);
        ~SphereRenderer();

    protected:
        void initialise_container(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices);
        unsigned int VBO, VAO, EBO;
        Shader *shader;
        int idxCount;
};