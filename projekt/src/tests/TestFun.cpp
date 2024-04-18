#include "tests/TestFun.h"

#include "Debug.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

namespace test
{
    TestFun::TestFun() :
        m_ClearColor { 0.0f, 0.0f, 0.0f, 1.0f },
        m_Positions {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        },
        m_Indices {
             0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
        },
        m_va(),
        m_ib(m_Indices, 36),
        m_vb(m_Positions, 6 * 5 * 6 * sizeof(float)),
        m_layout(),
        m_shader("resources/shaders/Fun.shader"),
        m_texture1("resources/textures/container.jpg"),
        m_texture2("resources/textures/phone.png"),
        m_renderer(),
        //m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_view(glm::mat4(1.0f)),
        m_translationA(0, 0, 0)
    {
        m_proj = glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far);

        m_layout.Push<float>(3);
        m_layout.Push<float>(2);
        m_va.AddBuffer(m_vb, m_layout);

        m_shader.Bind();
        m_texture1.Bind(0);
        m_texture2.Bind(1);
        m_shader.SetUniform1i("u_Texture1", 0);
        m_shader.SetUniform1i("u_Texture2", 1);
    }

        TestFun::~TestFun()
    {
        m_va.Unbind();
        m_shader.Unbind();
    }

    void TestFun::OnUpdate(float deltaTime)
    {
    }

    void TestFun::OnRender()
    {
        m_renderer.Clear();
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

        /*glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);*/
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //glm::mat4 model = glm::mat4(1.0f);
        m_proj = glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, m_view_val));
        glm::mat4 mvp = m_proj * m_view * model;

        m_shader.Bind();
        m_shader.SetUniformMat4f("u_MVP", mvp);

        m_renderer.Draw(m_va, m_ib, m_shader);        
    }

    void TestFun::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::SliderFloat("FOV", &m_fov, 0.0f, 90.0f);
        ImGui::SliderFloat("Near", &m_near, 0.1f, 20.0f);
        ImGui::SliderFloat("Far", &m_far, 25.0f, 300.0f);
        ImGui::SliderFloat("TranslationView", &m_view_val, -5.0f, 5.0f);
        //ImGui::SliderFloat3("TranslationA", &m_translationA.x, 0.0f, 960.0f);
    }
};
