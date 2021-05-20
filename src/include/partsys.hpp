#pragma once

#include <memory>
#include <vector>

#include "gl.hpp"
#include "particle.hpp"
#include "sceneobj.hpp"
// #include "swarm.hpp"
namespace swrm{class Swarm;}

class PartSys : public SceneObj
{
public:
    PartSys(int maxCount, Game *game);
    void draw() override;
    void update() override;
    void threadedSearch(uint32_t worker_id, uint32_t worker_count);

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
    std::vector<Particle> particles;
    std::vector<GLfloat> g_particule_position_size_data;
    std::vector<GLfloat> g_particule_color_data;
    swrm::Swarm *swarm;
};
