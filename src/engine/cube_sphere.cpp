#include "../includes/config.h"
#include "../includes/render/cube_sphere.h"

CubeSphere::CubeSphere(int LOD)
{
    // defining direction of faces
    std::vector<glm::vec3> faces = {
        glm::vec3{ 0.0f,  0.0f, -1.0f},   // front
        glm::vec3{-1.0f,  0.0f,  0.0f},   // left
        glm::vec3{ 1.0f,  0.0f,  0.0f},   // right
        glm::vec3{ 0.0f,  1.0f,  0.0f},   // top
        glm::vec3{ 0.0f, -1.0f,  0.0f},   // bottom
        glm::vec3{ 0.0f,  0.0f,  1.0f}    // back
    };

    for (int i = 0; i < 6; i++)
    {
        generate_face(faces[i], LOD);
    }
}

void CubeSphere::generate_face(glm::vec3 faceDir, int LOD)
{
    // assuming up is global up
    glm::vec3 axisA = glm::vec3{faceDir.y, faceDir.z, faceDir.x};
    glm::vec3 axisB = glm::cross(faceDir, axisA);

    float radius = 3.0f; // constant for now

    for (int i = 0; i < LOD; i++)
    {
        for (int j = 0; j < LOD; j++)
        {
            glm::vec2 point = glm::vec2{i, j} / (float)(LOD-1); // points normalised 0-1
            glm::vec3 vertex = faceDir + (point.x - 0.5f)*2.0f*axisA + (point.y - 0.5f)*2.0f*axisB;

            // projecting onto sphere
            glm::vec3 projection = radius*glm::normalize(vertex);
            this->vertices.push_back(projection);

            // indexing
            if (i < LOD - 1 && j < LOD - 1)
            {
                int index = vertices.size() - 1;
                this->indices.push_back(index);
                this->indices.push_back(index+LOD);
                this->indices.push_back(index+LOD+1);

                this->indices.push_back(index);
                this->indices.push_back(index+LOD+1);
                this->indices.push_back(index+1);
            }
        }
    }
}