#pragma once

#include <memory>
#include <vector>

#include "gl.hpp"
#include "particle.hpp"
#include "sceneobj.hpp"

class PartSys : public SceneObj
{
public:
    PartSys(int maxCount, glm::vec4 color, Game *game);
    void draw() override;
    void update() override;

private:
    void updateBuffers();
private:
    GLuint VAO;
    int maxCount;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLuint billboard_vertex_buffer;

    // GLfloat g_particule_position_size_data[400] = {0};
    // GLfloat g_particule_color_data[400] = {0};
	// GLfloat *g_particule_position_size_data;
    // GLfloat *g_particule_color_data;
    const glm::vec4 color;
    std::vector<Particle> particles;
    std::vector<GLfloat> g_particule_position_size_data;
    std::vector<GLfloat> g_particule_color_data;
};
