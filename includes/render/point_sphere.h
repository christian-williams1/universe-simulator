#pragma once

#include "../config.h"
#include "../includes/render/sphere_render.h"

class StarField
{
    public:
        StarField(Shader &shader);
        void draw();

    private:

        std::vector<glm::vec3> points;

        unsigned int VAO, VBO;

        Shader *shader;
};