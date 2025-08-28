//#include <GL/GL.h> //Legacy old Graphics library
#include <GLFW/glfw3.h> //Extensions of OpenGL and more modern Window functionality wrapper
//#include <GL/glu.h> Utility 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <vector>

static const float screenHeight = 900.0f;
static const float screenWidth = 1000.0f;



const char* vertSrc =
"#version 330 core"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 uMVP;\n"
"void main() { "
"    gl_Position = uMVP * vec4(aPos, 1.0);"
"}";

const char* fragSrc =
"#version 330 core"
"out vec4 FragColor"
"uniform vec3 uColor;"
"void main() {"
"   FragColor = vec4(uColor, 1.0);"
"}";

#if tempShaderUseless 0
GLuint Compileshader(GLenum type, const char* src)
{
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, nullptr);
	glCompileShader(s);
	GLint ok;
	glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		char buf[512]; glGetShaderInfoLog(s, 512, nullptr, buf);
		std::cerr << "Shader Compile Error: \n" << buf << "\n";
	}
	return s;
}
#endif
//Variables
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0;

//Functions
GLFWwindow* StartGLFW();
//Function Shapes

//Function Prototypes
void DrawCircle(float centerX, float centerY, float radius, int res);
void Triangle();
void MatrixUpdate();
void DrawCirclein3D(float centerX, float centerY, float centerZ , float radius, int segments = 64);

auto CameraLookMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 1.0, 0.0)

);

glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f); // Projection Matrix
/*
class Earth : Planet
{

};

class Planet
{

};
*/

class Object
{
public:
	
	std::vector<float> position;
	std::vector<float> velocity;
	float radius;

	Object(std::vector<float> position, std::vector<float> velocity, float radius = 15.0f)
	{
		this->position = position;
		this->velocity = velocity;
		this->radius = radius;
	}

	void accelerate(float x, float y)
	{
		velocity[0] += x;
		velocity[1] += y;
	}

	void updatePos()
	{
		position[0] += velocity[0];
		position[1] += velocity[1];
	}
	std::vector<float> DrawPlanet(int res = 10)
	{
		std::vector<float> vertices;

	}
	void DrawCircle(float centerX, float centerY, float radius, int res)
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
};

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = StartGLFW();
	if (!window) return -1;

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	std::vector<float> position = { 400.0f, 300.0f };
	//std::vector<float> velocity = { 0.0f, 0.0f };
	double lastTime = glfwGetTime();

	std::vector<float> velocity = { 0.0f, 50.0f };
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glLineWidth(2.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MatrixUpdate();
		//Triangle();
		DrawCirclein3D(0.0f, 0.0f, 0.0f, 1.5f, 128);



		glFlush();
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

	return window;
}

void DrawCirclein3D(float centerX, float centerY, float centerZ, float radius, int segments)
{

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		float theta = 2.0f * 3.14159265359f * float(i) / float(segments);
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		glVertex3f(centerX + x,  centerZ + z ,centerY );
	}
	glEnd();
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
void Triangle()
{
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
}
void DrawCircle3D(float centerX, float centerY, float centerZ, float radius, int segments)
{
	glColor3f(1.0f, 1.0f, 1.0f); // White color
	glBegin(GL_LINE_LOOP);      // Use LINE_LOOP to connect all points

	for (int i = 0; i < segments; ++i)
	{
		float theta = 2.0f * 3.14159265359f * float(i) / float(segments);
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		glVertex3f(centerX + x, centerY, centerZ + z); // Circle in XZ plane
	}

	glEnd();
}


void MatrixUpdate()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(glm::value_ptr(CameraLookMatrix));

	//glTranslatef(0.0f, 0.0f, -5.0f);
	//glRotatef(30.f, 1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glPopMatrix();
}