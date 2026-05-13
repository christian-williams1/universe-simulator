// this will include all keplerian elements
#pragma once

#include "../config.h"

struct KeplerianElements
{
    double eccentricity;
    double semiMajorAxis;
    double inclination;
    double ascendingNode;
    double periapsis;
};