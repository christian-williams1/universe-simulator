#include "../includes/simulation/orbit.h"
#include "../includes/math/math_utils.h"

// got much of the maths and theory from
// https://stjarnhimlen.se/comp/ppcomp.html

// values are currently hardcoded but needs to be not so hardcoded

Orbit::Orbit(double parentMass, double mass, KeplerianElements elements)
{
    // hardcoding properties at the moment
    elements.inclination += cfg::PI/2;
    
    //elements = {0.01, 100.0, 0.0, 0.0, 80.0};
    this->elements = elements;

    // assigning parent mass properties
    if (parentMass != 0.0)
    {
        // calculating mean motion
        this->mean = sqrt(cfg::G * parentMass / pow(abs(elements.semiMajorAxis), 3)); // n = sqrt(u/|a|^3)

        // calculating sphere of influence
        this->soi = elements.semiMajorAxis * pow(parentMass / mass, 0.4); // a*(M/m)^2/5
    }

    this->epoch = glfwGetTime();
}

glm::dvec3 Orbit::calculate_position()
{
    // this function gets the position of the next point in its orbit relative to the parent body
    float currentTime = glfwGetTime();

    double meanAnomaly = mean * (10*currentTime - epoch);

    double E = newtons_method(meanAnomaly, elements.eccentricity);

    // finding true anomaly along x and y axes
    double xv = elements.semiMajorAxis * (cos(E) - elements.eccentricity);
    double yv = elements.semiMajorAxis * (sqrt(1 - elements.eccentricity * elements.eccentricity) * sin(E));

    double r = sqrt(xv * xv + yv * yv);
    double v = atan2(yv, xv);

    double N = elements.ascendingNode;
    double w = elements.periapsis;
    double i = elements.inclination;

    double x = r * (cos(N) * cos(v + w) - sin(N) * sin(v + w) * cos(i));
    double y = r * (sin(N) * cos(v + w) + cos(N) * sin(v + w) * cos(i));
    double z = r * (sin(v + w) * sin(i));

    return glm::dvec3{x, y, z};
}