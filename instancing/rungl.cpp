
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

Camera camera(glm::vec3(0.0f, 0.0f,  80.0f));

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

	Shader instanceShader("./instance.vs", "./fragment.fs");

	Shader objShader("./vertex.vs", "./fragment.fs");
	objShader.use();

	// set the lights
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

	Model planet("../resources/planet/planet.obj");
	Model asteroid("../resources/rock/rock.obj");

	// asteroid transforms
	unsigned int amount = 20000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];

	srand(glfwGetTime()); // initialize random seed	
	float radius = 50.0;
	float offset = 2.5f;

	for(unsigned int i = 0; i < amount; i++)
	{
    	glm::mat4 model = glm::mat4(1.0f);
    	// 1. translation: displace along circle with 'radius' in range [-offset, offset]
    	float angle = (float)i / (float)amount * 360.0f;
    	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    	float x = sin(angle) * radius + displacement;
    	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    	float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
    	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    	float z = cos(angle) * radius + displacement;
    	model = glm::translate(model, glm::vec3(x, y, z));

    	// 2. scale: Scale between 0.05 and 0.25f
    	float scale = (rand() % 20) / 100.0f + 0.05;
    	model = glm::scale(model, glm::vec3(scale));

    	// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    	float rotAngle = (rand() % 360);
    	model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    	// 4. now add to list of matrices
    	modelMatrices[i] = model;
	}  

	// generate instance data
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
  
	for(unsigned int i = 0; i < asteroid.meshes.size(); i++)
	{
    	unsigned int VAO = asteroid.meshes[i].VAO;
    	glBindVertexArray(VAO);
    	// vertex Attributes
    	GLsizei vec4Size = sizeof(glm::vec4);
    	glEnableVertexAttribArray(3); 
    	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    	glEnableVertexAttribArray(4); 
    	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    	glEnableVertexAttribArray(5); 
    	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    	glEnableVertexAttribArray(6); 
    	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    	glVertexAttribDivisor(3, 1);
    	glVertexAttribDivisor(4, 1);
    	glVertexAttribDivisor(5, 1);
    	glVertexAttribDivisor(6, 1);

    	glBindVertexArray(0);
	}  

	while(!glfwWindowShouldClose(window)){ 
		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;  

		//rendering commands
		if (colorIndex == 1 )
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		else 
			glClearColor(0.3f, 0.2f, 0.2f, 1.0f);

		if (drawLines)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objShader.use();

		objShader.setVec3("viewPos", camera.getPos());
		objShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 400.0f);

		// send transformation matrices to shader
		objShader.setMat4("view", camera.getViewMatrix());
		objShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		objShader.setMat4("model", model);

		planet.Draw(objShader);

		// draw asteroids
		instanceShader.use();
		instanceShader.setMat4("view", camera.getViewMatrix());
		instanceShader.setMat4("projection", projection);

		for(unsigned int i = 0; i < asteroid.meshes.size(); i++)
		{
    		glBindVertexArray(asteroid.meshes[i].VAO);
    		glDrawElementsInstanced(
        		GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
    		);
		} 

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