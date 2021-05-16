#pragma once

#include "gl.hpp"
#include "sceneobj.hpp"
#include <memory>

class PartSys : public SceneObj
{
public:
    PartSys(int maxCount, Game *game);
    void draw() override;
    void update() override;

private:
    GLuint VAO;
    int maxCount;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLuint billboard_vertex_buffer;

    // GLfloat g_particule_position_size_data[400] = {0};
    // GLfloat g_particule_color_data[400] = {0};
	GLfloat *g_particule_position_size_data;
    GLfloat *g_particule_color_data;
};
