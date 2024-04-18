#include "tests/TestCube.h"

#include "Debug.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

namespace test
{
	TestCube::TestCube() :
		m_Positions {
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
		m_Indices {
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
		m_va(),
		m_vb(m_Positions, 8 * 3 * sizeof(float)),
		m_ib(m_Indices, 2 * 3 * 6),
		m_layout(),
		m_shader("resources/shaders/Cube.shader")
	{

		m_layout.Push<float>(3);
		m_va.AddBuffer(m_vb, m_layout);
	}

	TestCube::~TestCube()
	{
		m_va.Unbind();
		m_shader.Unbind();
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		
	}

	void TestCube::OnRender()
	{
		m_renderer.Clear();
		//GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		//glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		m_proj = glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far);
		m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_view_val));
		glm::mat4 mvp = m_proj * m_view * model;

		m_shader.Bind();
		m_shader.SetUniformMat4f("u_MVP", mvp);
		m_renderer.Draw(m_va, m_ib, m_shader);
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::SliderFloat("FOV", &m_fov, 0.0f, 90.0f);
		ImGui::SliderFloat("Near", &m_near, 0.1f, 20.0f);
		ImGui::SliderFloat("Far", &m_far, 25.0f, 300.0f);
		ImGui::SliderFloat("TranslationView", &m_view_val, -5.0f, 5.0f);
	}
};
