#include "../includes/simulation/orbit.h"

// values are currently hardcoded but needs to be not so hardcoded

Orbit::Orbit(float seed, Orbit &parent)
{
    // assigning parent node
    this->parent = &parent;

    // hardcoding properties at the moment
    this->mass = 1.0f;
    elements = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    // calculating mean motion
    this->mean = sqrt(cfg::G*mass/pow(abs(elements.semiMajorAxis), 3)); // n = sqrt(u/|a|^3)
    this->period = 2*cfg::PI/mean;

    // calculating sphere of influence
    this->soi = elements.semiMajorAxis*pow(parent.mass/mass, 0.4); // a*(M/m)^2/5
}