#include "Application.h"
#include <chrono>

using Clock = std::chrono::steady_clock;

Application::Application(GLFWwindow* windowHandle)
	:
	window{ windowHandle },
	playState{}
{

}

void Application::run()
{
	glClearColor(0.5, 0.65, 1.0, 1.0);	// temporary background/sky colour

	auto time = Clock::now();
	std::chrono::nanoseconds accumulator{ 0 };
	std::chrono::nanoseconds timestep{ 16666667 };	// 60 updates per second

	while (!glfwWindowShouldClose(window))
	{
		auto time2 = Clock::now();
		auto dt = time2 - time;
		time = time2;
		accumulator += dt;

		playState.processInput(window);		// input processing

		while (accumulator >= timestep)
		{
			accumulator -= timestep;
			playState.update(window);		// game state updating
		}

		playState.render(window);			// game state rendering
	}
}
