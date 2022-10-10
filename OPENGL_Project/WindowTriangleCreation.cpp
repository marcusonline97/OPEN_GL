#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Vertex Shader Source Code
const char* VertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader Source Code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"     FragColor = vec4(2.55f, 0.15f, 0.15f, 1.0f);\n"
"}\n\0";
int main()
{ //Initialize GLFW
	glfwInit();
	//Here we are telling GLFW which version of OpenGL we are using
	//In this case we are using OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW that we are using the Core Profile
	//so that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, +0.5f * float(sqrt(2)) / 3, 0.0f
	};
	GLFWwindow* window = glfwCreateWindow(800, 800, "My TestBox", NULL, NULL); //2 first values for the size of the window
	//ThirdValue is the text space for the name of the window. 

	if (window == NULL) {
		std::cout << "Failed to create Window." << std::endl; // Debug íf it was to fail to create a window.
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Create a window

	gladLoadGL(); //Load GLAD so it configures OGL
	//Specifying the viewport in OpenGl
	//In this case x = 0, y = 0, x = 800, y = 800.
	glViewport(0, 0, 800, 800); //Determining the size of the Window.


	//Create Vertex Shader Object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attach the Vertex Shader Source to the Vertex Shader Object 
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	//Compile the VertexShader into machine code.
	glCompileShader(vertexShader);
	//Create Fragment Shader Object and get a reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attach the Fragment Shader Source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compile the Fragment Shader into machine code.
	glCompileShader(fragmentShader);
	//Create Shader Program Object and get a reference
	GLuint shaderProgram = glCreateProgram();
	//Attach the Vertex Shader and Fragment Shader to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Wrap-in/Linking all the shaders togethere into the Shader Program
	glLinkProgram(shaderProgram);
	//Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//Create a reference container to the Vertex Array Object and Vertex Buffer Object
	GLuint VAO, VBO;
	//Generate the VAO and VBO with only 1 object each.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//Make the VAO the current Vertex Array Object by binding it.
	glBindVertexArray(VAO);
	//Bind the VBO specifying it's a Buffer Array.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glClearColor(0.87f, 0.63f, 0.37f, 1.0f); //The 3 first values signify RGB numbers and the last digit stand for the Alpha
	glClear(GL_COLOR_BUFFER_BIT); //We tell the program that we want to clear the original colour.

	glfwSwapBuffers(window); //Here we replace the windows base colour with the one we added above.


	while (!glfwWindowShouldClose(window)) 

		glClearColor(0.05f, 0.73f, 0.57f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		//Declare how many Arrays we want GL to draw.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		//Take care of the glfwEvents
		glfwPollEvents();
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}