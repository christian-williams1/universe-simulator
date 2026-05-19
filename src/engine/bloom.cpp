#include "../includes/render/bloom.h"
#include "../includes/config.h"

Bloom::Bloom(Shader &bloomShader, Shader &blurShader)
{
    this->bloomShader = &bloomShader;
    this->blurShader = &blurShader;

    // generating 2D Quad
    const std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f    // top right
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3};

    // generating 2D quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // generating FBO for postprocessing bloom effect
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // color buffers (1 for rendering, 1 for brightness)
    glGenTextures(2, colorBuffers);

    for (int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, (int)cfg::winWidth, (int)cfg::winHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Pingpong FBO incomplete!" << std::endl;
        }
    }

    attachments[0] = GL_COLOR_ATTACHMENT0;
    attachments[1] = GL_COLOR_ATTACHMENT1;

    // generating depth test RBO
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (int)cfg::winWidth, (int)cfg::winHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glDrawBuffers(2, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);

    for (int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (int)cfg::winWidth, (int)cfg::winHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Pingpong FBO incomplete!" << std::endl;
        }
    }
}

void Bloom::draw_quad()
{
    bool horizontal = true, first_iteration = true;
    int amount = 10;

    glUseProgram(blurShader->shaderID);

    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);

        blurShader->set_int("fboTex", 0);
        blurShader->set_int("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(bloomShader->shaderID);

    bloomShader->set_int("scene", 0);
    bloomShader->set_int("bloomBlur", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}