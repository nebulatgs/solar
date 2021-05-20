#include "include/particle.hpp"

Particle::Particle(glm::vec4 color) : disabled(false), color(color)
{
    // pos.x = ((emscripten_random() * 2.0) - 1.0) * 30.0;
    // pos.y = ((emscripten_random() * 2.0) - 1.0) * 30.0;
    pos = glm::diskRand(80.0);
    double dist = 100.0 / glm::distance(pos, {0,0}) + 100.0;
    vel = glm::normalize(glm::dvec2(pos.y, -pos.x)) * dist;
    // vel = {(glm::vec4(vel, 1.0, 1.0) * matRot).x, (glm::vec4(vel, 1.0, 1.0) * matRot).y};
    // vel.x = ((emscripten_random() * 2.0) - 1.0) / 1000.0;
    // vel.y = ((emscripten_random() * 2.0) - 1.0) / 1000.0;
    mass = (emscripten_random() * SizeRatio);

    size = mass / SizeRatio;
}
Particle::Particle(double mass, glm::vec4 color) : mass(mass), disabled(false), color(color)
{
    pos = {0, 0};
    vel = {0, 0};
    size = mass / (SizeRatio * 1000);
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
    pos += vel * TIMESTEP;
    force = {0, 0};
    // force *= 0;
}