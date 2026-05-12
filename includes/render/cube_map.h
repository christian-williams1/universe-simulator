#pragma once

#include "../config.h"
#include "../render/shader.h"

class SkyBox
{
    public:
        SkyBox();
        void draw(Shader &shader, glm::mat4 projection, glm::mat4 view);
    
    private:
        unsigned int texID, VAO, VBO, EBO;
};