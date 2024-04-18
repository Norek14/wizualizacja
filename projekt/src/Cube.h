#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"


class Cube
{
public:
	Cube();
	~Cube();

	inline VertexArray& GetVertexArray() { return m_vertexArray; }
	inline IndexBuffer& GetIndexBuffer() { return m_indexBuffer; }
	inline Shader& GetShader() { return m_shader; }
private:
	float m_positions[8 * 3];
	unsigned int m_indices[2 * 3 * 6];

	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	VertexBufferLayout m_layout;
	Shader m_shader;
};