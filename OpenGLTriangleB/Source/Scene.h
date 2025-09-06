#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "graphics/memory/framememory.hpp"
#include "graphics/memory/uniformmemory.hpp"

//Likely gonna add shaders and models here later

#include "io/camera.h"
#include "io/keyboard.h"
#include "io/mouse.h"


#include "algorithms/octree.h"
#include "algorithms/avl.h"
#include "algorithms/trie.h"

class Model;

class Scene
{
public:
	avl* models;
	trie::Trie<RigidBody*> instances;



	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	Scene();

	Scene(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int screenWidth, unsigned int screenHeight);


	bool init();

	void processInput(float dt);

	void update();

	void newFrame(Box& box);

	void renderShader(Shader shader, bool applyLightning = true);

	void cleanup();

	bool shouldClose();

	void setWindowColor(float r, float g, float b, float a);

	void initInstances();

	void removeInstance(std::string instanceId);

};