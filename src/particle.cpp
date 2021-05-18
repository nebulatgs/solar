#include "include/particle.hpp"

Particle::Particle() : disabled(false)
{
    pos.x = ((emscripten_random() * 2.0) - 1.0) * 30.0;
    pos.y = ((emscripten_random() * 2.0) - 1.0) * 30.0;

    vel.x = ((emscripten_random() * 2.0) - 1.0) / 10.0;
    vel.y = ((emscripten_random() * 2.0) - 1.0) / 10.0;

    mass = (emscripten_random() * SizeRatio);
}
Particle::Particle(double mass) : mass(mass), disabled(false)
{
    pos = {0, 0};
    vel = {2.0, 0};
    // pos.x = ((emscripten_random() * 2.0) - 1.0) * 100.0;
    // pos.y = ((emscripten_random() * 2.0) - 1.0) * 100.0;

    // vel.x = ((emscripten_random() * 2.0) - 1.0) / 10000.0;
    // vel.y = ((emscripten_random() * 2.0) - 1.0) / 10000.0;
}
void Particle::search(std::vector<Particle> &particles)
{
    for (auto &&item : particles)
    {
        double distance = glm::distance(item.pos, pos);
        if (distance > 4.0)
        {
            double pairForce = ((item.mass * mass) / (GCONSTANT * (distance * distance)));
            glm::dvec2 F = pairForce * (item.pos - pos) / distance;
            // if (glm::length(F) > 0.1)
            // {
            //     F = glm::normalize(F) * 0.1;
            // }
            force += F;
            item.force -= F;
        }
    }
}

void Particle::update()
{
    vel += force / mass;
    pos += vel;
    force = {0, 0};
    // force *= 0;
}