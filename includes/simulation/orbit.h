// this file is the header that defines the body class which contains a bodies attributes

#pragma once

#include "../config.h"
#include "kepler.h"

class Orbit
{
    public:
        Orbit(float seed, Orbit &parent);
    
    private:
        // planet phys properties
        float seed;
        double mass;
        double soi;

        // useful orbital elements
        double mean;
        double period;

        // keplerian elements/orbital elements
        struct KeplerianElements elements;
        std::vector<Orbit*> children;
        Orbit *parent;
};