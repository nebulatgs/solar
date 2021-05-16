#include "include/globals.hpp"
#include "include/game.hpp"
#include "include/globals.hpp"
#include "include/gl.hpp"

Game *game;

void error_callback(int error, const char *description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void tick()
{
	game->tick();
}

void getScreenSize(int &screen_width, int &screen_height)
{
	screen_width = EM_ASM_INT({
		var width = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;
		// var width = window.screen.width;
		return width;
	});
	screen_height = EM_ASM_INT({
		var height = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
		// var height = window.screen.height;
		return height;
	});
}

int main()
{
	int screen_width;
	int screen_height;
	getScreenSize(screen_width, screen_height);
	game = new Game(screen_width, screen_height, "Solar System");
	glfwSetErrorCallback(error_callback);

	emscripten_set_main_loop(tick, -1, 1);
	// while (!game->closed())
	// {
	// 	game->tick();
	// }
	glfwTerminate();
	exit(EXIT_SUCCESS);
}