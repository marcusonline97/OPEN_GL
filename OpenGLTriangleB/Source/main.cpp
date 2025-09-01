//#include <GL/GL.h> //Legacy old Graphics library
#include <glad/glad.h>
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

constexpr float TWO_PI = 2.0f * 3.14159265358979323846f;

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
bool running = true;
bool pause = true;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400.0, lastY = 300.0;
float yaw = -90;
float pitch = 0.0;
//Functions
GLFWwindow* StartGLFW();
void MatrixUpdate();

//Function Shapes
void renderSphere();
//Shader Program



//Cleaner to call OpenGL functions
void DeltaTime(); //Cleaner function to calculate delta time

void WindowInit(); //Cleaner function to set window hints

void Camera(GLuint shaderProgram, glm::vec3 cameraPos); //Cleaner function to set camera
//Function Prototypes
void DrawCircle(float centerX, float centerY, float radius, int res);
void Triangle();
void DrawCirclein3D(float centerX, float centerY, float centerZ, float radius, int segments = 64); // CPU function to draw a circle in 3D space

glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f); // Projection Matrix



class Planet
{
public:
	GLuint VAO, VBO;
	glm::vec3 position;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	size_t vertexCount;
	glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

};


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
	void WindowHint();

	GLFWwindow* window = StartGLFW();
	if (!window) return -1;



	double lastTime = glfwGetTime();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glLineWidth(2.0f);

	//Tons of boilerplate code to set up shaders and buffers



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DeltaTime();

		MatrixUpdate();
		//Triangle();
		void renderSphere();
		DrawCirclein3D(0.0f, 0.0f, 0.0f, 1.5f, 128);
		//DrawPlanetin3d(float radius, int segments);


		glFlush(); // Ensure all OpenGL commands are executed
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
	void WindowHint();

	GLFWwindow* window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "My Space Scene", nullptr, nullptr);

	if (!window) {
		std::cerr << "Failed to create GLFW Window.\n"; // Debug íf it was to fail to create a window.
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window); //Create a window

	
	GLFWwindow* current = glfwGetCurrentContext();
	std::cout << "Current context: " << current
		<< "  Expected window: " << window << std::endl;

	// 2) Before loading GLAD, check for any GLFW errors
	const char* errDesc = nullptr;
	int errCode = glfwGetError(&errDesc);
	std::cout << "GLFW error code: " << errCode
		<< "  Desc: " << (errDesc ? errDesc : "none") << std::endl;

	// 3) Attempt to load GLAD _once_
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	// 4) Print the OpenGL version string to confirm GLAD actually hooked in
	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	std::cout << "Loaded OpenGL version: "
		<< (version ? version : "NULL") << std::endl;

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

std::vector<float> DrawPlanetin3d(float centerX, float centerY, float centerZ, float radius, int segments)
{
	const int vertexCount = segments + 2;
	std::vector<float> vertices;
	vertices.reserve(vertexCount * 3); // 3 components per vertex (x, y, z)
	vertices.push_back(centerX); // Center vertex
	vertices.push_back(centerY);
	vertices.push_back(centerZ);

	for (int i = 0; i <= segments; ++i)
	{
		float t = float(i) / segments;
		float theta = t * TWO_PI;
		float x = centerX + radius * std::cos(theta) * radius;
		float z = centerZ + radius * std::sin(theta) * radius;

		vertices.push_back(x);
		vertices.push_back(centerY);
		vertices.push_back(z);
	}
	return vertices;
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

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void WindowInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}


void Camera(GLuint shaderProgram, glm::vec3 cameraPos)
{
//	glUseProgram(shaderProgram); // Shaders to apply
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
{
	return GLuint();
}

void CreateVBOVAO(GLuint& VBO, GLuint& VAO, const std::vector<float>& vertices)
{
}

void DeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void KeyboardInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void MatrixUpdate()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glLoadMatrixf(glm::value_ptr(CameraLookMatrix));

	//glTranslatef(0.0f, 0.0f, -5.0f);
	//glRotatef(30.f, 1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glPopMatrix();
}