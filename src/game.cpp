#include "include/game.hpp"
#include "include/partsys.hpp"

Game::Game(int width, int height, std::string title) : width(width), height(height)
{
    // Try to initialize GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Try to create a window
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GL_MAJOR_VERSION, 3);
    glfwWindowHint(GL_MINOR_VERSION, 1);
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
        exit(EXIT_FAILURE);

    // Load GL
    glfwMakeContextCurrent(window);
    // gladLoadGL();

    // Set viewport
    glfwGetFramebufferSize(window, &(this->width), &(this->height));
    glViewport(0, 0, this->width, this->height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, this->width, this->height, 0, 1, -1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearColor(0.176, 0.204, 0.212, 1.0);
    glfwSwapInterval(1);
    initGL();
}
void Game::initGL()
{
    game_objects.push_back(std::make_shared<PartSys>(1000, this));  
}

void Game::update()
{
    for (auto &&object : game_objects)
    {
        object->update();
    }
}

void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
    for (auto &&object : game_objects)
    {
        object->draw();
    }
    glfwSwapBuffers(window);
}

void Game::input()
{
}

void Game::tick()
{
    // double time = glfwGetTime();
    // double deltaTime = time - lastTime;
    // if (deltaTime >= 1.0 / 60.0)
    // {
        // lastTime = time;
        draw();
        update();
        input();
    // }
}