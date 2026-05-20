#include "../includes/game/player.h"

Player::Player(Body &body)
{
    this->mode = FREEMODE;
    this->body = &body;
    this->worldPos = glm::dvec3{0.0f, 10.0f, 0.0f};
}

void Player::check_soi()
{
    
}