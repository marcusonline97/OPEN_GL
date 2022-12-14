#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


const char* VertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"     FragColor = vec4(0.85f, 0.3f, 0.8f, 1.0f);\n"
"}\n\0";

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, +0.5f * float(sqrt(2)) / 3, 0.0f
	};
	GLFWwindow* window = glfwCreateWindow(800, 800, "My TestBox", NULL, NULL); //2 first values for the size of the window
	//Afterwards is a text space for the name of the window.

	if (window == NULL) {
		std::cout<< "Failed to create Window." << std::endl; // Debug of a failed window creation.
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Create a window

	gladLoadGL(); //Load the window
	
	glViewport(0, 0, 800, 800); //Determining the size of the Window.

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); 

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	///////////////////////////////////////////////////
	/// <VertexBuffers>
	/// <returns></returns>
	//
	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);


	glClearColor(0.87f, 0.63f, 0.37f, 1.0f); //The 3 first values signify RGB numbers and the last digit stand for the Alpha
	glClear(GL_COLOR_BUFFER_BIT); //We tell the program that we want to clear the original colour.
	
	glfwSwapBuffers(window); //Here we replace the windows base colour with the one we added above.


	while (!glfwWindowShouldClose(window)) { 
		glClearColor(0.05f, 0.73f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		//Take care of the glfwEvents
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	
	//Note to self (GL?M INTE STD:: !!!!! Vi ska inte bekanta oss med att anv?nda namespaces.
	
	return 0;
}