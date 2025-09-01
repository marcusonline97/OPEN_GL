#include "CircleMesh.h"
#include <cmath>

constexpr float TWO_PI = 2.0f * 3.14159265358979323846f;

std::vector<float> CircleMesh::makeCircleData(float centerX, float centerY, float centerZ, float radius, int segments)
{
	const int vertexCount = segments + 2;
	std::vector<float> vertices;
	vertices.reserve((segments + 2)* 3); // 3 components per vertex (x, y, z)

	vertices.insert(vertices.end(), { centerX, centerY, centerZ }); // Center vertex

	for (int i = 0; i <= segments; ++i)
	{
		float t = float(i) / segments;
		float theta = t * TWO_PI;
		float x = centerX + std::cos(theta) * radius;
		float z = centerZ + std::sin(theta) * radius;
		vertices.insert(vertices.end(), { x, centerY, z });
	}

	return vertices;
}

void CircleMesh::Init(
    float cx, float cy, float cz,
    float radius, int segments)
{
    // 2a) build CPU data
    auto data = makeCircleData(cx, cy, cz, radius, segments);
    vertexCount = GLsizei(data.size() / 3);

    // 2b) create & bind VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.size() * sizeof(float),
        data.data(),
        GL_STATIC_DRAW
    );

    // position attrib @location=0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CircleMesh::Draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    glBindVertexArray(0);
}

void CircleMesh::Cleanup()
{
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}