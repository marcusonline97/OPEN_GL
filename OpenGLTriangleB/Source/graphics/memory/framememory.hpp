#ifndef FRAMEMEMORY_HPP
#define FRAMEMEMORY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

//Rendering includes



class RenderbufferObject
{
public:
	GLuint val;

	void generate()
	{
		glGenRenderbuffers(1, &val);
	}

	void bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, val);
	}

	void allocate(GLenum format, GLuint width, GLuint height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	void cleanup()
	{
		glDeleteRenderbuffers(1, &val);
	}
};


#endif // !FRAMEMEMORY_HPP