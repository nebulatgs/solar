#include "include/partsys.hpp"
#include "include/game.hpp"
#include "include/shader.hpp"

PartSys::PartSys(int maxCount, glm::vec4 color, Game *game) : maxCount(maxCount), color(color), SceneObj(game)
{
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

    // particles.emplace_back(1000000);
    for (int i = 0; i < maxCount - 1; i++)
    {
        particles.emplace_back();
    }

    for (int i = 0; i < maxCount; i++)
    {
        g_particule_color_data.push_back(color.r);
        g_particule_color_data.push_back(color.g);
        g_particule_color_data.push_back(color.b);
        g_particule_color_data.push_back(color.a);
    }
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

void PartSys::update()
{
    int i = 0;
    for (auto &&particle : particles)
    {
        particle.update();
        g_particule_position_size_data[i] = particle.pos.x;
        g_particule_position_size_data[i + 1] = particle.pos.y;
        g_particule_position_size_data[i + 2] = 0;
        g_particule_position_size_data[i + 3] = particle.mass / Particle::SizeRatio;
        i += 4;
    }
    for (auto &&particle : particles)
    {
        particle.search(particles);
    }
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
        2,                // attribute. No particular reason for 2, but must match the layout in the shader.
        4,                // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_TRUE,          // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,                // stride
        (void *)0         // array buffer offset
    );

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxCount);
}