#pragma once

#include <string>
#include <vector>

#include "gl.hpp"
#include "sceneobj.hpp"

class Game
{
public:
    Game(int width, int height, std::string title);
    void tick();

    void update();
    void draw();
    void input();

public:
    glm::mat4 transform;

private:
    void initGL();

private:
    GLFWwindow *window;
    int width;
    int height;

private:
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    GLuint billboard_vertex_buffer;
    std::vector<std::shared_ptr<SceneObj>> game_objects;
};
