

#pragma once 
#include <vector>
#include <glad/glad.h>

struct CircleMesh
{
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLsizei vertexCount = 0;

	static std::vector<float> makeCircleData(float centerX, float centerY, float centerZ, float radius, int segments);

	void Init(float centerX, float centerY, float centerZ, float radius, int segments);

	void Draw() const;

	void Cleanup();

};