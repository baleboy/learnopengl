#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H 

#include <vector>
#include <glad/glad.h>

#include "sprite_renderer.h"
#include "game_object.h"

class GameLevel
{
public:
    std::vector<GameObject> Bricks;

    GameLevel() { }
    // Loads level from file
    void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    // Render level
    void Draw(SpriteRenderer &renderer);
    // Check if the level is completed (all non-solid tiles are destroyed)
    GLboolean IsCompleted();
private:
    // Initialize level from tile data
    void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};  

#endif