#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();

	void move(glm::vec3 movement);
	void rotate(glm::vec2 rotation);
	glm::mat4 getPerspective();
	glm::mat4 getView();
private:
	glm::vec3 lookAt;
	glm::vec3 position;
	glm::mat4 perspective;
	glm::mat4 view;
};

#endif // !CAMERA_H
