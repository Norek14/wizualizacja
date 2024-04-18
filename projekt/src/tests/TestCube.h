#pragma once
#include "Test.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	class TestCube : public Test
	{
    public:
        TestCube();
        ~TestCube();

        VertexArray& GetVertexArray() { return m_va; }
        IndexBuffer& GetIndexBuffer() { return m_ib; }
        Shader& GetShader() { return m_shader; }

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // data members
        float m_Positions[8 * 3];
        unsigned int m_Indices[2 * 3 * 6];

        // opengl members        
        VertexBuffer m_vb;
        VertexBufferLayout m_layout;
        VertexArray m_va;
        IndexBuffer m_ib;
        Shader m_shader;
        Renderer m_renderer;

        // MVP members
        glm::mat4 m_proj;
        glm::mat4 m_view;
        // model and mvp will be created per object per draw

        // Translation members
        glm::vec3 m_translationA;
        float m_fov = 45.0f;
        float m_near = 0.1f;
        float m_far = 100.0f;
        float m_view_val = 0.0f;
	};
}