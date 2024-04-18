#include "CommonEntity.h"
#include "LightingUtils.h"
#include "OBJ_Loader.h"

CommonEntity::CommonEntity(const std::string& model, const std::string& texture) :
    m_vertexArray(),
    m_layout(),
    m_shader("resources/shaders/Pin.shader"),
    m_texture(texture)
{
    objl::Loader loader;
    loader.LoadFile(model);

    objl::Mesh mesh = loader.LoadedMeshes.back(); // ostatni wczytany
    auto& material = mesh.MeshMaterial;
    // nie uzywane na razie
    ambient_color = glm::vec3(material.Ka.X, material.Ka.Y, material.Ka.Z);
    diffuse_color = glm::vec3(material.Kd.X, material.Kd.Y, material.Kd.Z);
    specular_color = glm::vec3(material.Ks.X, material.Ks.Y, material.Ks.Z);
    // 
    specular_exponent = material.Ns;

    unsigned int vertexBufferCount = mesh.Vertices.size() * 8;
    m_positions = new float[vertexBufferCount];

    int j = 0;
    for (int i = 0; i < mesh.Vertices.size(); i++)
    {
        m_positions[j++] = mesh.Vertices[i].Position.X;
        m_positions[j++] = mesh.Vertices[i].Position.Y;
        m_positions[j++] = mesh.Vertices[i].Position.Z;
        m_positions[j++] = mesh.Vertices[i].Normal.X;
        m_positions[j++] = mesh.Vertices[i].Normal.Y;
        m_positions[j++] = mesh.Vertices[i].Normal.Z;
        m_positions[j++] = mesh.Vertices[i].TextureCoordinate.X;
        m_positions[j++] = mesh.Vertices[i].TextureCoordinate.Y;
    }

    unsigned int indexBufferCount = mesh.Indices.size();
    m_indices = new unsigned int[indexBufferCount];
    for (int i = 0; i < indexBufferCount; i++)
    {
        m_indices[i] = mesh.Indices[i];
    }

    m_vertexBuffer = new VertexBuffer(m_positions, vertexBufferCount * sizeof(float));
    m_indexBuffer = new IndexBuffer(m_indices, indexBufferCount);

    m_layout.Push<float>(3); // pozycje
    m_layout.Push<float>(3); // normale
    m_layout.Push<float>(2); // tekstura
    m_vertexArray.AddBuffer(*m_vertexBuffer, m_layout);
    m_texture.Bind();
    m_shader.Bind();
    m_shader.SetUniform1i("material._texture", 0);
    m_shader.SetUniform1f("material.shininess", specular_exponent);
    LightingUtils::SetLightConstants(m_shader);
}

CommonEntity::~CommonEntity()
{
    m_vertexArray.Unbind();
    m_shader.Unbind();
    delete[] m_positions;
    delete[] m_indices;

    delete m_vertexBuffer;
    delete m_indexBuffer;
}