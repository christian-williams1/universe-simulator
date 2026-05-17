#include "../includes/render/cube_map.h"
#include "../dependencies/stb_image.h"
#include <string>

SkyBox::SkyBox()
{
    // defining texture locations
    std::vector<std::string> texLocations = {
        "../media/FaceOne.png", "../media/FaceSix.png",
        "../media/FaceThree.png", "../media/FaceFour.png",
        "../media/FaceFive.png", "../media/FaceTwo.png"
    };

    // generating texture buf
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    unsigned char *data;

    // iterating through each img
    for (unsigned int i = 0; i < 6; i++)
    {
        data = stbi_load(texLocations[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );

        if (!data)
        {
            std::cout << "FAILED: " << texLocations[i] << std::endl;
        }
        else
        {
            std::cout << "Loaded: " << texLocations[i] << std::endl;
        }
        stbi_image_free(data);
    }

    // applying to cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    this->texID = textureID;

    // creating vertices for cubemap
    // cube vertices
    float cubeScale = 1.0f;

    std::vector<float> vertices = {
        // Front face (z = +cubeScale)
         cubeScale,  cubeScale,  cubeScale,
         cubeScale, -cubeScale,  cubeScale,
        -cubeScale, -cubeScale,  cubeScale,
        -cubeScale,  cubeScale,  cubeScale,

        // Back face (z = -cubeScale)
         cubeScale,  cubeScale, -cubeScale,
         cubeScale, -cubeScale, -cubeScale,
        -cubeScale, -cubeScale, -cubeScale,
        -cubeScale,  cubeScale, -cubeScale,

        // Left face (x = -cubeScale)
        -cubeScale,  cubeScale,  cubeScale,
        -cubeScale, -cubeScale,  cubeScale,
        -cubeScale, -cubeScale, -cubeScale,
        -cubeScale,  cubeScale, -cubeScale,

        // Right face (x = +cubeScale)
        cubeScale,  cubeScale,  cubeScale,
        cubeScale, -cubeScale,  cubeScale,
        cubeScale, -cubeScale, -cubeScale,
        cubeScale,  cubeScale, -cubeScale,

        // Top face (y = +cubeScale)
        -cubeScale,  cubeScale,  cubeScale,
         cubeScale,  cubeScale,  cubeScale,
         cubeScale,  cubeScale, -cubeScale,
        -cubeScale,  cubeScale, -cubeScale,

        // Bottom face (y = -cubeScale)
         cubeScale, -cubeScale,  cubeScale,
        -cubeScale, -cubeScale,  cubeScale,
        -cubeScale, -cubeScale, -cubeScale,
         cubeScale, -cubeScale, -cubeScale,
    };

    std::vector<unsigned int> indices = {
        0,1,2,     0,2,3,          // front
        4,5,6,     4,6,7,          // back
        8,9,10,    8,10,11,        // left
        12,13,14,  12,14,15,       // right
        16,17,18,  16,18,19,       // top
        20,21,22,  20,22,23        // bottom
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

void SkyBox::draw(Shader &shader, glm::mat4 projection, glm::mat4 view)
{
    // disable depth test for cubemap
    glUseProgram(shader.shaderID);

    int projLoc = glGetUniformLocation(shader.shaderID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    view = glm::mat4(glm::mat3(view));
    int viewLoc = glGetUniformLocation(shader.shaderID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // MOVE OUT LATER
    glUniform1i(glGetUniformLocation(shader.shaderID, "skybox"), 0);
    glDepthMask(GL_FALSE);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
}