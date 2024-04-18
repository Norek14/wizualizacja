#include "Plane.h"
#include "LightingUtils.h"
#include "Debug.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include <GLFW/glfw3.h>

Plane::Plane() :
	m_positions {
		// positions            // normals         // texcoords
		 10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
				
		 10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	},
	m_indices {
		0, 1, 2, 3, 4, 5
	},
	m_vertexArray(),
	m_vertexBuffer(m_positions, 6 * 8 * sizeof(float)),
	m_indexBuffer(m_indices, 6),
	m_layout(),
	m_shader("resources/shaders/Plane.shader"),
	m_texture("resources/textures/wood.png")
{
	m_layout.Push<float>(3);
	m_layout.Push<float>(3);
	m_layout.Push<float>(2);
	m_vertexArray.AddBuffer(m_vertexBuffer, m_layout);

	m_texture.Bind();
	m_shader.Bind();
	m_shader.SetUniform1i("material._texture", 0);
	m_shader.SetUniform1f("material.shininess", 32.0);
	LightingUtils::SetLightConstants(m_shader);
}

Plane::~Plane()
{
	m_vertexArray.Unbind();
	m_shader.Unbind();
}
