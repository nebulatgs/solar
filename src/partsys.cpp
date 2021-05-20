#include "include/partsys.hpp"
#include "include/game.hpp"
#include "include/shader.hpp"
#include "include/swarm.hpp"

PartSys::PartSys(int maxCount, Game *game) : maxCount(maxCount), SceneObj(game)
{
    swarm = new swrm::Swarm(8);
    g_particule_position_size_data.reserve(maxCount * 4);
    g_particule_color_data.reserve(maxCount * 4);
    particles.reserve(maxCount);
    shader.Compile("shaders/circle.vert", "shaders/circle.frag");
    static const GLfloat vertices[] = {
        -1.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        0.0f,
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);

    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    particles.emplace_back(5000000, glm::vec4(0.0, 0.722, 0.58, 1.0));
    g_particule_color_data.push_back(particles.back().color.r);
    g_particule_color_data.push_back(particles.back().color.g);
    g_particule_color_data.push_back(particles.back().color.b);
    g_particule_color_data.push_back(particles.back().color.a);
    for (int i = 0; i < maxCount - 1; i++)
    {
        particles.emplace_back((i % 3 > 1 ? glm::vec4(0.424, 0.361, 0.906, 0.75) : glm::vec4(0.035, 0.518, 0.89, 0.75)));
        g_particule_color_data.push_back(particles.back().color.r);
        g_particule_color_data.push_back(particles.back().color.g);
        g_particule_color_data.push_back(particles.back().color.b);
        g_particule_color_data.push_back(particles.back().color.a);
    }

    // for (int i = 0; i < maxCount; i++)
    // {
    //     g_particule_color_data.push_back(color.r);
    //     g_particule_color_data.push_back(color.g);
    //     g_particule_color_data.push_back(color.b);
    //     g_particule_color_data.push_back(color.a);
    // }
}

void PartSys::updateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, maxCount * sizeof(GLfloat) * 4, g_particule_position_size_data.data());

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, maxCount * sizeof(GLfloat) * 4, g_particule_color_data.data());
}

void PartSys::threadedSearch(uint32_t worker_id, uint32_t worker_count)
{
    // Number of values for which the thread is responsible
    const uint32_t step = particles.size() / worker_count;
    // First value for the thread
    const uint32_t start_index = worker_id * step;
    // Last value
    const uint32_t end_index = (worker_id < worker_count - 1) ? start_index + step : particles.size() - 1;

    // The actual loop
    for (uint32_t i(start_index); i < end_index; ++i)
    {
        particles[i].search(particles);
    }
}

void PartSys::update()
{
    int i = 0;
    for (auto &&particle : particles)
    {
        particle.update();
        g_particule_position_size_data[i] = particle.pos.x;
        g_particule_position_size_data[i + 1] = particle.pos.y;
        g_particule_position_size_data[i + 2] = 0;
        g_particule_position_size_data[i + 3] = particle.size; // / Particle::SizeRatio;
        i += 4;
    }
    swrm::WorkGroup work_group = swarm->execute([&](uint32_t worker_id, uint32_t worker_count)
                                                { this->threadedSearch(worker_id, worker_count); });
    work_group.waitExecutionDone();
    // for (auto &&particle : particles)
    // {
    //     particle.search(particles);
    // }
    game->transform = glm::translate(game->transform, {particles.front().pos, 0.0});
    updateBuffers();
}

void PartSys::draw()
{
    shader.SetMatrix4("transform", game->transform, true);
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(
        0,        // attribute. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        4,        // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(
        2,        // attribute. No particular reason for 2, but must match the layout in the shader.
        4,        // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE,  // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,        // stride
        (void *)0 // array buffer offset
    );

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxCount);
}