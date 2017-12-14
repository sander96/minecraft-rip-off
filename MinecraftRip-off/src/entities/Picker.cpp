#include "Picker.h"
#include <iostream>
Picker::Picker() 
{
	currentRay = glm::vec3(0, 0, 0);
}
void Picker::update()
{
	viewMatrix = camera.getView();
	currentRay = calculateMouseRay();
}
glm::vec2 getNormDV(double mx, double my, int aX, int aY) {
	double x = (2.0f*mx) / aX - 1;
	double y = (2.0f*my) / aY - 1;
	glm::vec2 valja = glm::vec2(x, y);
	return valja;
}
glm::vec4 toEyeCord(glm::vec4 clipcord, glm::mat4 proj) {
	glm::mat4 invertedProjection = glm::inverse(proj);
	glm::vec4 eyeCord = invertedProjection*clipcord;
	return glm::vec4(eyeCord.x, eyeCord.y, -1, 0);
}
glm::vec3 toWorldCord(glm::vec4 eyeCord,glm::mat4 view) {
	glm::mat4 invertedView = glm::inverse(view);
	glm::vec4 rayWorld = invertedView * eyeCord;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	glm::normalize(mouseRay);
	return mouseRay;
}
glm::vec3 Picker::calculateMouseRay() {
	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	int ajuX;
	int ajuY;
	glfwGetWindowSize(window, &ajuX, &ajuY);
	glm::vec2 normalized = getNormDV(mouseX, mouseY, ajuX, ajuY);
	glm::vec4 clipCord = glm::vec4(normalized.x, normalized.y, -1, 1);
	glm::vec4 eyeSpace = toEyeCord(clipCord, projectionMatrix);
	glm::vec3 worldRay = toWorldCord(eyeSpace, viewMatrix);
	return worldRay;
}



