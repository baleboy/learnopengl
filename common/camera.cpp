#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.h>


Camera::Camera(const glm::vec3 &cameraPos) 
{
	this->cameraPos = cameraPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
	cameraSpeed = 2.5f;
	firstMouse = true;
	lastX = 0.0f;
	lastY = 0.0f;
	sensitivity = 0.05;
	yaw = -90.0f;
	pitch = 0.0f; 
	fov = 45.0;
}

void Camera::moveForward(float deltaTime)
{
	cameraPos += cameraSpeed * deltaTime * cameraFront;
}

void Camera::moveBackward(float deltaTime)
{
   	cameraPos -= cameraSpeed * deltaTime * cameraFront;
}

void Camera::moveLeft(float deltaTime)
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
}

void Camera::moveRight(float deltaTime)
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;	
}

void Camera::processMouse(float xpos, float ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::processScroll(float yoffset)
{
	if(fov >= 1.0f && fov <= 45.0f)
  		fov -= yoffset;
  	else if(fov < 1.0f)
  		fov = 1.0f;
  	else if(fov > 45.0f)
  		fov = 45.0f;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::getFov() const
{
	return fov;
}

glm::vec3 Camera::getPos() const
{
  return cameraPos;
}

glm::vec3 Camera::getFront() const
{
  return cameraFront;
}





