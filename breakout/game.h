#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <tuple>

#include "sprite_renderer.h"
#include "game_level.h"
#include "ball_object.h"

class Game 
{
	public:

		// Represents the current state of the game
		enum GameState {
    		GAME_ACTIVE,
    		GAME_MENU,
    		GAME_WIN
		};
		
		GameState	State;
		GLboolean	Keys[1024];
		GLuint		Width, Height;

   		std::vector<GameLevel> Levels;
    	GLuint                 Level;

		Game(GLuint width, GLuint height);
		~Game();	

		// Initialize game state, load resources
		void Init();

		// Game loop
		void ProcessInput(GLfloat dt);
		void Update(GLfloat dt);
		void Render();
		void DoCollisions();

	private:

		enum Direction {
			UP,
			RIGHT,
			DOWN,
			LEFT
		};

		typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; 

		SpriteRenderer* Renderer;
		GameObject* Player;
		BallObject* Ball;

		Game::Collision checkCollision(BallObject &one, GameObject &two);
		Game::Direction vectorDirection(glm::vec2 target);
};

#endif 