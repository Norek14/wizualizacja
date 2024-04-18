#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include <vector>

class Sphere
{
public:
	Sphere();
	~Sphere();

	inline VertexArray& GetVertexArray() { return m_vertexArray; }
	inline IndexBuffer& GetIndexBuffer() { return *m_indexBuffer; }
	inline Shader& GetShader() { return m_shader; }
private:
	float* m_positions;
	unsigned int* m_indices;

	void BuildVerticesSmooth();
	void AddVertex(float x, float y, float z);
	void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	void AddNormal(float nx, float ny, float nz);
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::vector<unsigned int> m_vec_indices;
	float m_radius = 1.0f;
	int m_sectorCount = 36;
	int m_stackCount = 18;
	VertexArray m_vertexArray;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;
	VertexBufferLayout m_layout;
	Shader m_shader;
};