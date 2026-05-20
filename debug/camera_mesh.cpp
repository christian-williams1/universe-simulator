#include "camera_mesh.h"

CameraMesh::CameraMesh(Shader &shader)
{
    this->shader = &shader;

    float cubeScale = 1.0f;

    std::vector<float> vertices = {
        -1.0f, -1.0f, -1.0f, // 0 back bottom left
        1.0f, -1.0f, -1.0f, // 1 back bottom right
        1.0f,  1.0f, -1.0f, // 2 back top right
        -1.0f,  1.0f, -1.0f, // 3 back top left
        -1.0f, -1.0f,  1.0f, // 4 front bottom left
        1.0f, -1.0f,  1.0f, // 5 front bottom right
        1.0f,  1.0f,  1.0f, // 6 front top right
        -1.0f,  1.0f,  1.0f, // 7 front top left
    };

    std::vector<unsigned int> indices = {
        // back face
        0,1,2, 0,2,3,
        // front face
        4,5,6, 4,6,7,
        // left face
        0,4,7, 0,7,3,
        // right face
        1,5,6, 1,6,2,
        // bottom face
        0,1,5, 0,5,4,
        // top face
        3,2,6, 3,6,7,
    };

    // binding and initialising cube

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void CameraMesh::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 &debugProjection, glm::mat4 &debugView)
{
    glUseProgram(shader->shaderID);
    glBindVertexArray(VAO);

    // creating frustum
    glm::vec3 color = glm::vec3(0.5f, 0.7f, 0.9f);
    
    int vertexColorLoc = glGetUniformLocation(shader->shaderID, "col");
    glUniform3f(vertexColorLoc, color.x, color.y, color.z);

    glm::mat4 invViewProj = glm::inverse(projection*view);

    glm::mat4 model = glm::mat4(1.0f);
    int modelLoc = glGetUniformLocation(shader->shaderID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(invViewProj));

    // sending debug cameras uniforms
    int projectionLoc = glGetUniformLocation(shader->shaderID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(debugProjection));

    int viewLoc = glGetUniformLocation(shader->shaderID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(debugView));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
}