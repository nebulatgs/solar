#pragma once

#include "gl.hpp"
#include "sceneobj.hpp"
#include <memory>

class BController : public SceneObj
{
public:
    BController(int count, Game *game);
    void draw() override;
    void update() override;

private:
    GLuint VAO;
    int count;
    GLuint billboard_vertex_buffer;
};