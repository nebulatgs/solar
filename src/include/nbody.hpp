#pragma once

#include "bcontroller.hpp"

class NBody
{
public:
    NBody(int count, BController *controller);
    void draw();
    void update();

private:
    std::shared_ptr<BController> controller;
    GLuint VAO;
    int count;
    GLuint billboard_vertex_buffer;
};