#include "../includes/game/player.h"

Player::Player(Body &body)
{
    this->mode = FREEMODE;
    this->body = &body;
    this->worldPos = glm::dvec3{1000.0f, 0.0f, 0.0f};
}

// getters

bool Player::get_local_space()
{
    return localSpace;
}

Body *Player::get_body()
{
    return body;
}

// this method checks if the player is within the critical radius.
// if it is then we can render local space
void Player::check_critical_radius()
{
    if (glm::distance(worldPos, body->position) < 2*body->get_size()*criticalRadiusScalar)
    {
        localSpace = true;
    }
    else
    {
        localSpace = false;
    }
}

// this function checks which SOI the player is currently in
void Player::check_soi()
{
    if (localSpace) return;

    // checking if outside current SOI radius
    if (glm::distance(worldPos, body->position) > body->orbit->get_soi())
    {
        // settomg old body to red
        body->set_colour(glm::vec3{1.0f, 0.0f, 0.0f});
        body = body->parent;
        body->set_colour(glm::vec3{0.0f, 1.0f, 0.0f});
        return;
    }

    // checking if in childs SOI
    for (auto &child : body->children)
    {
        if (glm::distance(worldPos, child->position) < child->orbit->get_soi())
        {
            // debugging
            // set old body to red
            body->set_colour(glm::vec3{1.0f, 0.0f, 0.0f});
            body = child;
            child->set_colour(glm::vec3{0.0f, 1.0f, 0.0f});
            return;
        }
    }
}