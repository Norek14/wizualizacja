#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"


class Plane
{
public:
	Plane();
	~Plane();

	inline VertexArray& GetVertexArray() { return m_vertexArray; }
	inline IndexBuffer& GetIndexBuffer() { return m_indexBuffer; }
	inline Shader& GetShader() { return m_shader; }
	inline Texture& GetTexture() { return m_texture; }
private:
	float m_positions[6 * 8];
	unsigned int m_indices[6];

	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	VertexBufferLayout m_layout;
	Texture m_texture;
	Shader m_shader;
};