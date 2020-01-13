
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "stb_image.h"

unsigned int colorIndex= 1;
bool drawLines = false;
bool drawSquare = true;

Camera camera(glm::vec3(0.0f, 0.0f,  6.0f));

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void loadTextures(unsigned int& texture1, unsigned int& texture2);
unsigned int createCube();

int main()
{
	glfwInit();

  	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
#ifdef OSX
	/* We need to explicitly ask for a 3.2 context on OS X */
  	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 #else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
  	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback); 


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);  

	// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightPos(-1.2f, 0.0f, 2.0f);

	Shader objShader("./vertex.vs", "./fragment.fs");
	objShader.use();
	objShader.setVec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
	objShader.setVec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
	objShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); 
	objShader.setVec3("material.ambient",  glm::vec3(1.0f, 0.5f, 0.31f));
	objShader.setVec3("material.diffuse",  glm::vec3(1.0f, 0.5f, 0.31f));
	objShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	objShader.setFloat("material.shininess", 32.0f);

	Shader lightShader("./lightVertex.vs", "./light.fs");

	unsigned int cubeVAO = createCube();
	unsigned int lightVAO = createCube();

	while(!glfwWindowShouldClose(window)){ 
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;  

		//rendering commands
		if (colorIndex == 1 )
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		else 
			glClearColor(0.3f, 0.2f, 0.2f, 1.0f);

		if (drawLines)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		// Draw cube
		objShader.use();

		objShader.setVec3("viewPos", camera.getPos());
		objShader.setVec3("light.position", lightPos);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
  
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
  
		objShader.setVec3("light.ambient", ambientColor);
		objShader.setVec3("light.diffuse", diffuseColor);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

		// send transformation matrices to shader
		objShader.setMat4("view", camera.getViewMatrix());
		objShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		objShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw light

		lightShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); 
		lightShader.setMat4("view", camera.getViewMatrix());
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("model", model);
		lightShader.setVec3("lightColor", lightColor);

		glBindVertexArray(cubeVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		colorIndex = 1;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		colorIndex = 2;

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		drawLines = !drawLines;	

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(deltaTime);	
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.processMouse(xpos, ypos);
}  

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processScroll(yoffset);
}

void loadTextures(unsigned int& texture1, unsigned int& texture2)
{
	// could have been an array of two ints with first param '2'
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture 1" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);  
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    	glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

}



unsigned int createCube()
{
	float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  

	// vertex normal attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  

	return VAO;
}
