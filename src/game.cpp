#include "include/globals.hpp"
#include "include/game.hpp"
#include "include/partsys.hpp"
#include "include/bcontroller.hpp"


void resizeCallback(GLFWwindow* window, int width, int height)
{
	game->draw();
}

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
	glfwSetWindowSizeCallback(window, resizeCallback);
    // gladLoadGL();

    // Set viewport
    glfwGetFramebufferSize(window, &(this->width), &(this->height));
	aspect = static_cast<float>(height) / static_cast<float>(width);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glClearColor(0.176, 0.204, 0.212, 1.0);
    glfwSwapInterval(1);
    transform = glm::mat4(1.0);
    transform = glm::scale(transform, {aspect, 1.0, 1.0});
    // transform = glm::scale(transform, {0.5, 0.5, 1.0});
    initGL();
}

void Game::initGL()
{
    game_objects.push_back(std::make_shared<PartSys>(10, this));  
}

void Game::update()
{
	int screen_width, screen_height;
	getScreenSize(screen_width, screen_height);
	glfwSetWindowSize(window, screen_width, screen_height);
	glViewport(0, 0, screen_width, screen_height);
    glfwGetFramebufferSize(window, &(this->width), &(this->height));
	aspect = static_cast<float>(height) / static_cast<float>(width);
	transform = glm::mat4(1.0);
    transform = glm::scale(transform, {aspect, 1.0, 1.0});
    // transform = glm::scale(transform, {0.5, 0.5, 1.0});
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
        update();
        draw();
        input();
    // }
}