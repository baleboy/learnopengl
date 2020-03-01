#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>

#include "sprite_renderer.h"

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

		Game(GLuint width, GLuint height);
		~Game();	

		// Initialize game state, load resources
		void Init();

		// Game loop
		void ProcessInput(GLfloat dt);
		void Update(GLfloat dt);
		void Render();

	private:
		SpriteRenderer* Renderer;
};

#endif 