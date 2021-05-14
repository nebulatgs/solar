#pragma once

#include <memory>
#pragma once

#include "shader.hpp"
class Game;

class SceneObj
{
public:
    SceneObj(Game *game) : game(game) {}
    virtual void draw() {}
    virtual void update() {}

protected:
    Game *game;
    Shader shader;
};
