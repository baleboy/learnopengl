#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"
#include "shader.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 9.5f;
		
Game::Game(GLuint width, GLuint height)
{
	Width = width;
	Height = height;
}

Game::~Game() 
{
}	


void Game::Init()
{
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    this->Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    // Load textures
    ResourceManager::LoadTexture("images/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("images/ball.png", GL_TRUE, "ball");
    ResourceManager::LoadTexture("images/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("images/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("images/paddle.png", true, "paddle");

    // Load levels
    GameLevel one; one.Load("levels/level1.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("levels/level2.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("levels/level3.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("levels/level4.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    glm::vec2 playerPos = glm::vec2(
    	this->Width / 2 - PLAYER_SIZE.x / 2, 
        this->Height - PLAYER_SIZE.y
    );

    this->Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = glm::vec2(
    	this->Width / 2 - BALL_RADIUS, 
        this->Height - PLAYER_SIZE.y - 2*BALL_RADIUS
    );

    this->Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY);
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // Move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0) 
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
   		{
   			this->Ball->Stuck = false;
   		}
   		     	
    }

}

void Game::Update(GLfloat dt)
{
	this->Ball->Move(dt, this->Width);
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), 
            glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
        );
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        this->Player->Draw(*Renderer);
        this->Ball->Draw(*Renderer);
    }
}