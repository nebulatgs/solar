#include "include/globals.hpp"
#include "include/game.hpp"
#include "include/globals.hpp"
#include "include/gl.hpp"

Game* game;

void error_callback(int error, const char *description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void tick()
{
	game->tick();
}

int main()
{
	game = new Game(720, 720, "Solar System");
	glfwSetErrorCallback(error_callback);
	
	emscripten_set_main_loop(tick, -1, 1);
	// while (!game->closed())
	// {
	// 	game->tick();
	// }
	glfwTerminate();
	exit(EXIT_SUCCESS);
}