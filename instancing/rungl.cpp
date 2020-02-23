
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <stb_image.h>
#include <mesh.h>
#include <model.h>

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

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
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

	glm::vec3 lightPos(-1.2f, 0.0f, 2.0f);
	glm::vec3 lightColor(1.0, 1.0, 1.0);

	Shader objShader("./vertex.vs", "./fragment.fs");
	objShader.use();

	// set the lights
	objShader.setVec3("flashLight.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
	objShader.setVec3("flashLight.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
	objShader.setVec3("flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f)); 
	objShader.setFloat("material.shininess", 32.0f);

	objShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	objShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	objShader.setVec3("dirLight.diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
	objShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f)); 

	glm::vec3 pointLightPositions[] = {
		glm::vec3( 0.7f,  0.2f,  -5.0f),
		glm::vec3( 2.3f, -3.3f, -8.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -15.0f)
	};  

	#define NUM_POINT_LIGHTS 4

	for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
	{
		std::string base = "pointLights[" + std::to_string(i) + "].";

		objShader.setVec3(base + "position",  pointLightPositions[i]);		
		objShader.setVec3(base + "ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
		objShader.setVec3(base + "diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
		objShader.setVec3(base + "specular", glm::vec3(1.0f, 1.0f, 1.0f)); 
		objShader.setFloat(base + "constant",  1.0f);
		objShader.setFloat(base + "linear",    0.09f);
		objShader.setFloat(base + "quadratic", 0.032f);
	}

	Model nanosuit("../resources/nanosuit/nanosuit.obj");

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

		// Draw cube
		objShader.use();

		objShader.setVec3("viewPos", camera.getPos());
		objShader.setVec3("flashLight.position", camera.getPos());
  
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
  
		objShader.setVec3("flashLight.ambient", ambientColor);
		objShader.setVec3("flashLight.diffuse", diffuseColor);
		objShader.setVec3("flashLight.direction", camera.getFront());
		objShader.setFloat("flashLight.cutOff",   glm::cos(glm::radians(12.5f)));
		objShader.setFloat("flashLight.outerCutOff",   glm::cos(glm::radians(17.5f)));

		objShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

		// send transformation matrices to shader
		objShader.setMat4("view", camera.getViewMatrix());
		objShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		objShader.setMat4("model", model);

		nanosuit.Draw(objShader);

		glfwSwapBuffers(window);		
		glfwPollEvents();

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