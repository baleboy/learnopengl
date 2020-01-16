#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
	public:

		Camera(const glm::vec3 &cameraPos);

		void moveForward(float deltaTime);
		void moveBackward(float deltaTime);
		void moveLeft(float deltaTime);
		void moveRight(float deltaTime);

		void processMouse(float xpos, float ypos);

		void processScroll(float yoffset);

		glm::mat4 getViewMatrix() const;

		float getFov() const;
		glm::vec3 getPos() const;

	private:

		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		float cameraSpeed;
		bool firstMouse;
		float lastX;
		float lastY;
		float sensitivity;
		float yaw;
		float pitch;
		float fov;
};

#endif