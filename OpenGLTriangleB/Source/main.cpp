#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <vector>


float screenHeight = 600.0f;
float screenWidth = 800.0f;

GLFWwindow* StartGLFW();
void DrawCircle(float centerX, float centerY, float radius, int res);


int main()
{
	GLFWwindow* window = StartGLFW();
	if (!window) return -1;

	float centerX = screenWidth / 2;
	float centerY = screenHeight / 2;
	float radius = 50.0f;
	int res = 100;

	std::vector<float> position = { 400.0f, 300.0f };
	std::vector<float> velocity = { 0.0f, 0.0f };
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{

		double now = glfwGetTime();
		float deltaTime = static_cast<float>(now - lastTime);
		lastTime = now;

		glClear(GL_COLOR_BUFFER_BIT);

		DrawCircle(position[0], position[1], 50.0f, 50);

		position[0] += velocity[0];
		position[1] += velocity[1];
		velocity[1] -= 9.81f * deltaTime * 10; // Gravity effect

		if (position[1] < 0 || position[1] > screenHeight)
		{
			velocity[1] *= -0.95;
		}
		if(position[0] < 0 || position[0] > screenWidth)
		{
			velocity[0] *= -0.95;
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}

GLFWwindow* StartGLFW()
{
	if (!glfwInit())
	{
		std::cerr << "failed to Initialize GLFW" << std::endl;
		return nullptr;
	}

	GLFWwindow* window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "My Space Scene", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create Window." << std::endl; // Debug íf it was to fail to create a window.
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window); //Create a window

	glViewport(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight)); //Determining the size of the Window.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black Background
	return window;
}



void DrawCircle (float centerX, float centerY, float radius, int res)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(centerX, centerY);
	for (int i = 0; i <= res; i++)
	{
		float angle = 2.0f * 3.14159f * (static_cast<float>(i) / res);
		float x = centerX + cos(angle) * radius;
		float y = centerY + sin(angle) * radius;
		glVertex2d(x, y);
	}
	glEnd();
}