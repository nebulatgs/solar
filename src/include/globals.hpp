#pragma once

#include <emscripten.h>
#include <glm/ext.hpp>
// #include <glm/gtc/matrix_transform.hpp>

class Game;
extern Game* game;

extern void getScreenSize(int &screen_width, int &screen_height);