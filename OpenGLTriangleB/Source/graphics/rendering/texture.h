#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assimp/scene.h>

#include "stb/stb_image.h"

class Texture
{
public:

	Texture(std::string name);

	Texture(std::string dir, std::string path, aiTextureType type);

	void generate();

	void load(bool flip = true);

	void allocate(GLenum format, GLuint width, GLuint height, GLenum type);

	static void setParams(GLenum texMinFilter = GL_NEAREST,
		GLenum texMagFiler = GL_NEAREST,
		GLenum wrapS = GL_REPEAT, 
		GLenum wrapT = GL_REPEAT);

	void bind();

	void cleanup();

	unsigned int id;

	aiTextureType type;

	std::string name;

	std::string dir;

	std::string path;
};

#endif // !TEXTURE_H