#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <string>
#include <utility>

class CommonEntity
{
public:
    CommonEntity(const std::string& model, const std::string& texture);
    virtual ~CommonEntity();

    virtual std::pair<std::string, std::string> GetModelAndTexturePaths() const = 0;

    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float specular_exponent;

    inline VertexArray& GetVertexArray() { return m_vertexArray; }
    inline IndexBuffer& GetIndexBuffer() { return *m_indexBuffer; }
    inline Shader& GetShader() { return m_shader; }
    inline Texture& GetTexture() { return m_texture; }

protected:
    float* m_positions;
    unsigned int* m_indices;

    VertexArray m_vertexArray;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    VertexBufferLayout m_layout;
    Shader m_shader;
    Texture m_texture;

    std::string m_modelPath;
    std::string m_texturePath;
};
