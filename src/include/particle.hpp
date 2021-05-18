#pragma once

#include <vector>

#include "globals.hpp"

struct Particle
{
    Particle();
    Particle(double mass);
    void update();
    void search(std::vector<Particle>& particles);
    
    static double constexpr G = 6.6742E-11;
    static double constexpr SizeRatio = 7500.0;
    static double constexpr GCONSTANT = 1.00E4;
    double mass;
    bool disabled;
    glm::dvec2 pos;
    glm::dvec2 vel;
    glm::dvec2 force;
};
