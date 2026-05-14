#include "../includes/simulation/body.h"

Body::Body(Body *parent, glm::vec3 color, KeplerianElements elements)
{
    if (parent != NULL)
    {
        this->parent = parent;
        this->mass = 1.0f;
        this->size = 100.0f;
        this->orbit = new Orbit(parent->mass, mass, elements);
    }
    else
    {
        this->mass = 1.0f;
        this->orbit = new Orbit(0.0, mass, elements);
        this->position = glm::vec3{0.0f};
        this->size = 10.0f;
    }
    
    //this->size = 3.0f;
    this->colour = color;
}

double Body::get_size()
{
    return this->size;
}

glm::vec3 Body::get_color()
{
    return this->colour;
}

void Body::orbit_traverse(glm::vec3 &position)
{
    this->position = orbit->calculate_position() + position;

    for (auto &child : children)
    {
        child->orbit_traverse(this->position);
    }
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