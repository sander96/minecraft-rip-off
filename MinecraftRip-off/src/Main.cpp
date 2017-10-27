#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "utilities/Settings.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	Settings settings;
	window = glfwCreateWindow(settings.getWidth(), settings.getHeight(), "Hello World", NULL, NULL);	// maybe lock the resolution?

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(0);	// disable vsync
	std::cout << glGetString(GL_VERSION) << std::endl;

	Application application(window);
	application.run();		// all game logic

	glfwTerminate();
}