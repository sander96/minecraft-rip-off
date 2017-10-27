#include "Application.h"

Application::Application(GLFWwindow* windowHandle)
	:
	window{ windowHandle }
{

}

void Application::run()
{
	glClearColor(0.5, 0.65, 1.0, 1.0);	// temporary background/sky colour

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);

		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}
