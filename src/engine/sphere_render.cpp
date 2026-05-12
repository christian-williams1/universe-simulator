#include "../includes/render/sphere_render.h"

SphereRenderer::SphereRenderer(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices, glm::vec3 color)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    this->idxCount = indices.size(); // storing index count for static geometry of cube sphere
    this->color = color;
}

void SphereRenderer::draw(const Shader &shader, glm::vec3 position)
{
    glUseProgram(shader.shaderID);
    
    glBindVertexArray(VAO);
    int vertexColorLoc = glGetUniformLocation(shader.shaderID, "col");
    glUniform3f(vertexColorLoc, color.x, color.y, color.z);

    int modelLoc = glGetUniformLocation(shader.shaderID, "model");
    glm::mat4 model = glm::mat4{1.0f};
    model = glm::translate(model, position);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
}

SphereRenderer::~SphereRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}