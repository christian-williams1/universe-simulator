#pragma once

#include "../config.h"
#include "../includes/render/shader.h"

class Bloom
{
    public:
        Bloom(Shader &bloomShader, Shader &blurShader);

        void draw_quad();

        unsigned int FBO;

    private:
        unsigned int RBO, VAO, VBO, EBO;
        unsigned int colorBuffers[2], attachments[2];
        unsigned int pingpongFBO[2];
        unsigned int pingpongBuffer[2];
        unsigned int idxCount = 6;
        Shader *bloomShader;
        Shader *blurShader;
};