#include "include/globals.hpp"

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

