
#include "ball_object.h"
#include "resource_manager.h"

BallObject::BallObject()
: Radius(1.0f), Stuck(true)
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity)
:   GameObject(pos, glm::vec2(radius*2, radius*2), ResourceManager::GetTexture("ball"), glm::vec3(1.0f), velocity),
    Radius(radius), Stuck(true)
{

}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
    // If not stuck to player board
    if (!this->Stuck)
    { 
        // Move the ball
        this->Position += this->Velocity * dt;
        // Check if outside window bounds; if so, reverse velocity and restore at correct position
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
      
    }
    return this->Position;
}  