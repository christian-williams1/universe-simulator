#include "../includes/math/math_utils.h"

double newtons_method(double M,  double e)
{
    double soln[2] = {0, 0};

    // calculating first approximation
    soln[0] = M + e*sin(M) * (1.0 + e*cos(M));
    
    for (int i = 0; i < 10; i++)
    {
        soln[1] = soln[0] - (soln[0]-M-e*sin(soln[0]))/(1-e*cos(soln[0]));

        if (abs(soln[1] - soln[0]) < 1e-6)
        {
            break;
        }

        soln[0] = soln[1];
    }

    return soln[1];
}