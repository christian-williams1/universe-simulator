// this file is the header that defines the body class which contains a bodies attributes

#pragma once

#include "../config.h"

class Body
{
    public:
        Body();
        float get_radius();

        // holds pointer to 'parent' body
        Body *parent;
        Body *child;

    private:
        float radius;
        float mass;

};