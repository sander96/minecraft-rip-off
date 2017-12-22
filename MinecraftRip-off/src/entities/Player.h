#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "../Camera.h"
#include "../utilities/Settings.h"
#include <iostream>
#include "../block/Block.h"

class Player
{
public:
	Player(glm::vec3 pos);
	void processInput(GLFWwindow* window);

	glm::vec3 getPosition() { return position; }
	glm::mat4 getCameraPerspective() { return camera.getPerspective(); };
	glm::mat4 getCameraView() { return camera.getView(); }
	glm::vec3 getCameraDirection() { return camera.getDirection(); }

	bool removeBlockEvent() { return removeBlock; }
	bool addBlockEvent() { return addBlock; }

	Block getCurrentBlock() { return currentBlock; }

private:
	void processMovement(GLFWwindow* window);
	void processMouse(GLFWwindow* window);

	void move(glm::vec3 movement);
	void rotate(glm::vec2 rotation);

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 leftVector;
	Camera camera;
	Settings settings;

	bool removeBlock = false;
	bool addBlock = false;

	unsigned int leftMouseDelay = 0;
	unsigned int rightMouseDelay = 0;

	Block currentBlock = Block::Dirt;
};

#endif
