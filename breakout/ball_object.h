#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"

class BallObject : public GameObject
{
    public:
        // Ball state	
        GLfloat   Radius;
        GLboolean Stuck;
  

        BallObject();
        BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

        glm::vec2 Move(GLfloat dt, GLuint window_width);
        void      Reset(glm::vec2 position, glm::vec2 velocity);
}; 

#endif