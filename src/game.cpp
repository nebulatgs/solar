#include "include/globals.hpp"
#include "include/game.hpp"
#include "include/partsys.hpp"
#include "include/bcontroller.hpp"

void mouseCallback(GLFWwindow *, int button, int action, int)
{
    game->input(button, action);
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
	game->draw();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset > 0)
        game->zoomCamera(0.75);
    else
        game->zoomCamera(1.333); 
    // emscripten_log(0, "%f", yoffset101.0);
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
    
    // glClearColor(0.176, 0.204, 0.212, 1.0);
    glClearColor(0.082, 0.098, 0.102, 1.0);
    glfwSwapInterval(1);
    camTransform = glm::mat4(1.0);
    transform = glm::scale(camTransform, {aspect, 1.0, 1.0});
    // transform = glm::scale(transform, {0.5, 0.5, 1.0});
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scroll_callback);
    initGL();
	// glClear(GL_COLOR_BUFFER_BIT);
    // glfwSwapBuffers(window);
	// glClear(GL_COLOR_BUFFER_BIT);
}

void Game::initGL()
{
    game_objects.push_back(std::make_shared<PartSys>(1000, this));  
    // game_objects.push_back(std::make_shared<PartSys>(1000, glm::vec4(0.298, 0.82, 0.216, 0.75), this));
}

void Game::update()
{
	int screen_width, screen_height;
	getScreenSize(screen_width, screen_height);
	glfwSetWindowSize(window, screen_width, screen_height);
	glViewport(0, 0, screen_width, screen_height);
    glfwGetFramebufferSize(window, &(this->width), &(this->height));
	aspect = static_cast<float>(height) / static_cast<float>(width);
    transform = camTransform;
    // if(mouseDown)
    // {
    //     double x, y;
    //     glfwGetCursorPos(window, &x, &y);
    //     emscripten_log(0, "%f, %f", x, y);
    //     transform = glm::translate(transform, {0.0f,0.0f,1.0f});
    // }
    transform = glm::scale(transform, {aspect, 1.0, 1.0});
    transform = glm::scale(transform, {0.01, 0.01, 1.0});
    transform = glm::translate(transform, {10.0,10.0,10.0});
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
    // glAccum(GL_ACCUM, 1.0f);
    // glAccum(GL_RETURN, 1.0f);
    glfwSwapBuffers(window);
}

void Game::input(int button, int action)
{
    this->button = button;
    this->mouseDown = action;
    if(mouseDown)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        startMousePos = {x, y};
    }
}

void Game::input()
{
}

void Game::zoomCamera(float zoom)
{
    camTransform = glm::scale(camTransform, {zoom, zoom, 1.0f});
    // camTransform *= glm::translate(glm::mat4(1.0), {100.0, 100.0, 100.0});
    // camTransform = glm::translate(camTransform, {100.0f,100.0f,100.0f});
    // emscripten_log(0, "%f", camTransform.length());
    // glm::tr
}

void Game::tick()
{
    // double time = glfwGetTime();
    // double deltaTime = time - lastTime;
    // if (deltaTime >= 1.0 / 60.0)
    // {

        // lastTime = time;
        for(int i = 0; i < 6; i++)
            update();
        draw();
        input();
    // }
}