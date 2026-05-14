#include "../includes/simulation/body.h"

Body::Body(Body *parent, glm::vec3 color, KeplerianElements elements)
{
    if (parent != NULL)
    {
        this->parent = parent;
        this->orbit = new Orbit(parent->mass, mass, elements);
        //this->position = glm::vec3{5.0f};
    }
    else
    {
        //this->orbit = new Orbit(0.0, mass);
        this->position = glm::vec3{0.0f};
    }
    this->size = 3.0f;
    this->colour = color;
    this->mass = 1.0f;
}

double Body::get_size()
{
    return this->size;
}

glm::vec3 Body::get_color()
{
    return this->colour;
}

glm::vec3 Body::next_position()
{
    position = orbit->calculate_position();

    return position + parent->position;
}

Body::~Body()
{
    delete this->orbit;
    this->orbit = nullptr;
}