#include "Cube.h"

#include "Debug.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

Cube::Cube() :
	m_positions {
		// front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	},
	m_indices {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	},
	m_vertexArray(),
	m_vertexBuffer(m_positions, 8 * 3 * sizeof(float)),
	m_indexBuffer(m_indices, 2 * 3 * 6),
	m_layout(),
	m_shader("resources/shaders/Cube.shader")
{

	m_layout.Push<float>(3);
	m_vertexArray.AddBuffer(m_vertexBuffer, m_layout);
}

Cube::~Cube()
{
	m_vertexArray.Unbind();
	m_shader.Unbind();
}
