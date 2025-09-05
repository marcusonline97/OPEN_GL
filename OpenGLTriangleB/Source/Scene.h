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



class Model;

class Scene
{
public:
	avl* models;
	trie::Trie<RigidBody*> instances;
};