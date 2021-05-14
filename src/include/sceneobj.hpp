#pragma once

#include <memory>

#include "shader.hpp"

class Game;

class SceneObj
{
public:
    SceneObj(Game *game) : game(game) {}
    virtual void draw() {}
    virtual void update() {}

protected:
    std::shared_ptr<Game> game;
    Shader shader;
};
