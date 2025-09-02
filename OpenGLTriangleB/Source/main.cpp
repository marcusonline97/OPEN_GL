//#include <GL/GL.h> //Legacy old Graphics library
#include <glad/glad.h>
#include <GLFW/glfw3.h> //Extensions of OpenGL and more modern Window functionality wrapper
//#include <GL/glu.h> Utility 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>
#include <cmath>
#include <vector>


// --- Function Declarations ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* StartGLFW();
GLuint compileShader(GLenum type, const char* src);
GLuint createShaderProgram(const char* vertSrc, const char* fragSrc);
void setupTriangle(GLuint& VAO, GLuint& VBO);
void processInput(GLFWwindow* window);
glm::mat4 computeMVP(GLFWwindow* window, float angle);
//Window settings
const unsigned int screenHeight = 720;
const unsigned int screenWidth = 1280;


const char* vertSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
uniform mat4 uMVP;
out vec2 TexCoord;
void main() { 
    gl_Position = uMVP * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}
)";

const char* fragSrc = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D uTexture;
void main() {
   FragColor = texture(uTexture, TexCoord);
}
)";

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
float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight); // Aspect ratio
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f); // Projection matrix
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400.0, lastY = 300.0;
float yaw = -90;
float pitch = 0.0;
//Functions
void MatrixUpdate();
//Function Shapes
void renderSphere();
//Shader Program
GLuint compileShader(GLenum type, const char* src);
GLuint createShaderProgram(const char* vSrc, const char* fSrc);
//Texture Loader
GLuint loadTexture(const char* path);
//Cleaner to call OpenGL functions
void DeltaTime(); //Cleaner function to calculate delta time
void WindowInit(); //Cleaner function to set window hints
//Function Prototypes
void Triangle();

//--- Declaration Functions ---
GLFWwindow* StartGLFW() // Works fine
{
	WindowInit();

	if (!glfwInit())
	{
		std::cerr << "failed to Initialize GLFW" << std::endl;
		return nullptr;
	}


	GLFWwindow* window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "My Space Scene", nullptr, nullptr);
	glfwMakeContextCurrent(window); //Create a window

	if (!window) {
		std::cerr << "Failed to create GLFW Window.\n"; // Debug íf it was to fail to create a window.
		glfwTerminate();
		return nullptr;
	}


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

GLuint compileShader(GLenum type, const char* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cerr << "Shader compile error:\n" << log << "\n";
	}
	return shader;
}

GLuint createShaderProgram(const char* vSrc, const char* fSrc) {
	GLuint vs = compileShader(GL_VERTEX_SHADER, vSrc);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fSrc);
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	GLint ok;
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok) {
		char log[512];
		glGetProgramInfoLog(prog, 512, nullptr, log);
		std::cerr << "Program link error:\n" << log << "\n";
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	return prog;
}

GLuint loadTexture(const char* path)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip the image vertically
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture: " << path << std::endl;
		return 0;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return texture;
}

void setupTriangle(GLuint& VAO, GLuint& VBO) {
	float verts[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}





int main()
{
	GLFWwindow* window = StartGLFW();
	if (!window) return -1;

	GLuint VAO, VBO;
	setupTriangle(VAO, VBO);

	GLuint shader = createShaderProgram(vertSrc, fragSrc);
	GLuint earthTexture = loadTexture("flat_earth.jpg");
	GLint locMVP = glGetUniformLocation(shader, "uMVP");
	//GLint locColor = glGetUniformLocation(shader, "uColor");


	while (!glfwWindowShouldClose(window))
	{
		DeltaTime(); // Calculate delta time

		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float angle = glfwGetTime();

		glm::mat4 mvp = computeMVP(window, angle);

		glUseProgram(shader);
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
		//glUniform3f(locColor, 0.0f, 1.0f, 0.0f);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glBindVertexArray(0);

		//MatrixUpdate();
		//Triangle();
		glUseProgram(shader);
		glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthTexture);
		glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);
		renderSphere();
		//DrawCirclein3D(0.0f, 0.0f, 0.0f, 1.5f, 128);
		//DrawPlanetin3d(float radius, int segments);


		glFlush(); // Ensure all OpenGL commands are executed
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}



void Triangle()
{
	float triangleVertices[] = 
		{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
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
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

GLuint CompileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Error checking
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cerr << "Shader compile error:\n" << log << std::endl;
	}
	return shader;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
glm::mat4 computeMVP(GLFWwindow* window, float angle) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	float aspect = float(w) / float(h);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),  // camera pos
		glm::vec3(0.0f, 0.0f, 0.0f),  // target
		glm::vec3(0.0f, 1.0f, 0.0f)   // up direction
	);
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle,
		glm::vec3(0.0f, 1.0f, 0.0f));
	return proj * view * model;
}
void MatrixUpdate()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glLoadMatrixf(glm::value_ptr(CameraLookMatrix));

	//glTranslatef(0.0f, 0.0f, -5.0f);
	//glRotatef(30.f, 1.0f, 0.0f, 0.0f);
	//glPushMatrix();
	//glPopMatrix();
}