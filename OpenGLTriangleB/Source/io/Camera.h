#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>





enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};;

class Camera
{
public: 
	glm::vec3 cameraPos;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	float yaw;
	float pitch;

	float speed;
	float sensitivity;
	float zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f));

	void updateCameraDirection(double dx, double dy);

	void updateCameraPos(CameraDirection direction, double deltaTime);

	void updateCameraZoom(double dy);

	glm::mat4 getViewMatrix();

	float getZoom();

private:
	void updateCameraVectors();

};

#endif // !CAMERA_H
