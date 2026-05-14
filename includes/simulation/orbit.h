// this file is the header that defines the body class which contains a bodies attributes

#pragma once

#include "../config.h"
#include "kepler.h"

class Orbit
{
    public:
        Orbit(double parentMass, double mass, KeplerianElements elements);
        glm::vec3 calculate_position();
    
    private:
        // planet phys properties
        float seed;
        double soi;

        // useful orbital elements
        double mean;
        float epoch;

        // keplerian elements/orbital elements
        struct KeplerianElements elements;
        Orbit *parent;
};