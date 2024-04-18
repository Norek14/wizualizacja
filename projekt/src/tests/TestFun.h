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
    class TestFun : public Test
    {
        public:
            TestFun();
            ~TestFun();

            void OnUpdate(float deltaTime) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            // data members
            float m_ClearColor[4];
            float m_Positions[180];
            unsigned int m_Indices[36];

            // opengl members
            VertexArray m_va;
            IndexBuffer m_ib;
            VertexBuffer m_vb;
            VertexBufferLayout m_layout;
            Shader m_shader;
            Texture m_texture1;
            Texture m_texture2;
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
