#pragma once

#include "../includes/config.h"
#include "../includes/render/shader.h"

class CameraMesh
{
    public:
        CameraMesh(Shader &shader);
        void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 &debugProjection, glm::mat4 &debugView);
    private:
        unsigned int VAO, VBO, EBO;
        Shader *shader;
};