#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "../Camera.h"
#include "../utilities/Settings.h"
#include "Picker.h"

class Player
{
public:
	Player(glm::vec3 pos);
	void processInput(GLFWwindow* window);
	int getSeis() { return seis; };

	glm::vec3 getPosition() { return position; }
	glm::mat4 getCameraPerspective() { return camera.getPerspective(); };
	glm::mat4 getCameraView() { return camera.getView(); }
	void SetEemaldus() { seis = -1; };
	void SetLisamine() { seis = 1; };
	void SetMidagi() { seis = 0; };
	void SetRay(glm::vec3 lisa) {
		ray = lisa;
	};
	glm::vec3 getRay() {
		return ray;
	};

private:
	void move(glm::vec3 movement);
	void rotate(glm::vec2 rotation);

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 leftVector;
	Camera camera;
	Settings settings;
	int seis;
	glm::vec3 ray;
	Picker picker;
};

#endif
