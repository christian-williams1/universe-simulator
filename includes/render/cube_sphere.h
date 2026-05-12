#pragma once
#include "../config.h"

class CubeSphere
{
    public:
        CubeSphere(int LOD);
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;

    private:
        void generate_face(glm::vec3 faceDirection, int LOD);
};