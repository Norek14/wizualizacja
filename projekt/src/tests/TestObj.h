#pragma once

#include "Test.h"
#include "TestCube.h"
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
    class TestObj : public Test
    {
    public:
        TestObj();
        ~TestObj();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // data members
        float m_ClearColor[4];
        float *m_Positions;
        unsigned int *m_Indices;

        // opengl members
        VertexArray m_va;
        IndexBuffer* m_ib = nullptr;
        VertexBuffer* m_vb = nullptr;
        VertexBufferLayout m_layout;
        Shader m_shader;
        Texture m_texture;
        Renderer m_renderer;

        // MVP members
        glm::mat4 m_proj;
        glm::mat4 m_view;
        // model and mvp will be created per object per draw

        // Translation members
        glm::vec3 m_translationA;
        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
        float m_fov = 45.0f;
        float m_near = 0.1f;
        float m_far = 100.0f;
        glm::vec3 m_view_val = glm::vec3(0.0f, 0.0f, 0.0f);
        
        TestCube cube;
    };
}
