#include "tests/TestObj.h"

#include "Debug.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include "OBJ_Loader.h"

namespace test
{
    TestObj::TestObj() :
        m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f },        
        m_va(),
        m_layout(),
        m_shader("resources/shaders/Obj.shader"),
        m_texture("resources/textures/capsule0.jpg"),
        m_renderer(),
        m_view(glm::mat4(1.0f)),
        m_translationA(0, 0, 0)
    {
        objl::Loader loader;
        loader.LoadFile("resources/models/10492_Bowling Pin_v1_max2011_iteration-2.obj");
        objl::Mesh mesh = loader.LoadedMeshes.back(); // ostatni wczytany
        auto& material = mesh.MeshMaterial;
        auto& testol = loader.LoadedMaterials;
        //unsigned int vertexBufferCount = mesh.Vertices.size() * 3;
        unsigned int vertexBufferCount = mesh.Vertices.size() * 6;
        m_Positions = new float[vertexBufferCount];

        int j = 0;
        for (int i = 0; i < mesh.Vertices.size(); i++)
        {
            m_Positions[j++] = mesh.Vertices[i].Position.X;
            m_Positions[j++] = mesh.Vertices[i].Position.Y;
            m_Positions[j++] = mesh.Vertices[i].Position.Z;
            m_Positions[j++] = mesh.Vertices[i].Normal.X;
            m_Positions[j++] = mesh.Vertices[i].Normal.Y;
            m_Positions[j++] = mesh.Vertices[i].Normal.Z;
        }

        unsigned int indexBufferCount = mesh.Indices.size();
        m_Indices = new unsigned int[indexBufferCount];
        for (int i = 0; i < indexBufferCount; i++)
        {
            m_Indices[i] = mesh.Indices[i];
        }

        m_vb = new VertexBuffer(m_Positions, vertexBufferCount * sizeof(float));
        m_ib = new IndexBuffer(m_Indices, indexBufferCount);

        m_proj = glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far);

        m_layout.Push<float>(3); // pozycje
        m_layout.Push<float>(3); // normale
        m_va.AddBuffer(*m_vb, m_layout);

        m_shader.Bind();
        m_shader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        m_shader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
        m_shader.SetUniform3f("objectColor", 0.2, 0.3, 0.8);
    }

    TestObj::~TestObj()
    {
        m_va.Unbind();
        m_shader.Unbind();
        delete[] m_Positions;
        delete[] m_Indices;
        delete m_vb;
        delete m_ib;
    }

    void TestObj::OnUpdate(float deltaTime)
    {
    }

    void TestObj::OnRender()
    {
        m_renderer.Clear();
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

        /*glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);*/
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f));
        //glm::mat4 model = glm::mat4(1.0f);
        m_proj = glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far);
        m_view = glm::translate(glm::mat4(1.0f), m_view_val);
        glm::mat4 mvp = m_proj * m_view * model;        

        m_shader.Bind();
        m_shader.SetUniformMat4f("model", model);
        m_shader.SetUniformMat4f("u_MVP", mvp);

        m_shader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        m_renderer.Draw(m_va, *m_ib, m_shader);
		/*glm::vec3 cubePositions[] = {
	        glm::vec3(2.0f,  5.0f, -15.0f),
	        glm::vec3(-1.5f, -2.2f, -2.5f),
	        glm::vec3(-3.8f, -2.0f, -12.3f) 
        };
        for (int i = 1; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i - 1]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            mvp = m_proj * m_view * model;
            m_shader.SetUniformMat4f("model", model);
            m_shader.SetUniformMat4f("u_MVP", mvp);
            m_renderer.Draw(m_va, *m_ib, m_shader);
        }*/
        

        glm::mat4 cube_model = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.1f));
        //glm::mat4 cube_model = glm::mat4(1.0f);
        glm::mat4 cube_mvp = m_proj * m_view * cube_model;
        auto& cube_shader = cube.GetShader();
        cube_shader.Bind();
        cube_shader.SetUniformMat4f("u_MVP", cube_mvp);

        m_renderer.Draw(cube.GetVertexArray(), cube.GetIndexBuffer(), cube_shader);
    }

    void TestObj::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::SliderFloat("FOV", &m_fov, 0.0f, 90.0f);
        ImGui::SliderFloat("Near", &m_near, 0.1f, 20.0f);
        ImGui::SliderFloat("Far", &m_far, 25.0f, 300.0f);
        ImGui::SliderFloat3("TranslationView", &m_view_val[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("TranslationCube", &lightPos[0], -10.0f, 10.0f);
        //ImGui::SliderFloat3("TranslationA", &m_translationA.x, 0.0f, 960.0f);
    }
};
