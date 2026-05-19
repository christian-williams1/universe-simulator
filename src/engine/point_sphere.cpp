#include "../includes/render/point_sphere.h"
#include <cstdlib>

StarField::StarField(Shader &shader)
{
    this->shader = &shader;

    int starCount = 1000;

    // generating random engine
    glm::vec3 rndSet;
    glm::vec3 point;

    for (int i = 0; i < 500; i++)
    {
        float x = ((rand() % 201) / 100.0f) - 1.0f;
        float y = ((rand() % 201) / 100.0f) - 1.0f;
        float z = ((rand() % 201) / 100.0f) - 1.0f;

        rndSet = glm::vec3(x, y, z);
        point = glm::normalize(rndSet);

        this->points.push_back(point);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void StarField::draw()
{
    glUseProgram(shader->shaderID);

    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, points.size());

    glBindVertexArray(0);
}