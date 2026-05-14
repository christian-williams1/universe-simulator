#include "../includes/game/player.h"

Player::Player(Body &body)
{
    this->mode = FREEMODE;
    this->body = &body;
    this->worldPos = glm::dvec3{0.0f};
}

void Player::switch_body()
{
    //if (body)
}