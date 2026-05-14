// This file contains parameters to do with planetary bodies, natural satellites, etc.

#pragma once

#include "../config.h"
#include "../includes/simulation/orbit.h"

class Body
{
    public:
        Body(Body *parent, glm::vec3 color, KeplerianElements elements);
        double get_size();
        glm::vec3 get_color();

        glm::vec3 next_position();

        ~Body();

        Orbit *orbit;
        glm::vec3 position;
    
    private:
        std::vector<Body *> children;
        Body *parent;

        double size;
        double mass;
        glm::vec3 colour;
};