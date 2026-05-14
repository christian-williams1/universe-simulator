#pragma once

#include "../config.h"
#include "./simulation/body.h"

typedef enum {
    FREEMODE,
    GRAVITY
} Mode;

class Player
{
    public:
        Player(Body &body);
        void switch_body();

        glm::dvec3 worldPos;
    private:
        Body *body;
        Mode mode;
};