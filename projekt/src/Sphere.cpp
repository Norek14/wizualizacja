#include "Sphere.h"
#include "LightingUtils.h"

Sphere::Sphere() :
    m_vertexArray(),
    m_layout(),
    m_shader("resources/shaders/Sphere.shader")
{
    BuildVerticesSmooth();
    unsigned int vertexBufferCount = m_vertices.size() * 2; // wierzch + normale
    m_positions = new float[vertexBufferCount];

    int j = 0; 
    for (int i = 0; i < vertexBufferCount / 2; i += 3)
    {
        m_positions[j++] = m_vertices[i];
        m_positions[j++] = m_vertices[i + 1];
        m_positions[j++] = m_vertices[i + 2];
        m_positions[j++] = m_normals[i];
        m_positions[j++] = m_normals[i + 1];
        m_positions[j++] = m_normals[i + 2];
    }

    unsigned int indexBufferCount = m_vec_indices.size();
    m_indices = new unsigned int[indexBufferCount];
    for (int i = 0; i < indexBufferCount; i++)
    {
        m_indices[i] = m_vec_indices[i];
    }

    m_vertexBuffer = new VertexBuffer(m_positions, vertexBufferCount * sizeof(float));
    m_indexBuffer = new IndexBuffer(m_indices, indexBufferCount);

    m_layout.Push<float>(3); // pozycje
    m_layout.Push<float>(3); // normale
    m_vertexArray.AddBuffer(*m_vertexBuffer, m_layout);
    LightingUtils::SetLightConstants(m_shader);
}

Sphere::~Sphere()
{
    m_vertexArray.Unbind();
    m_shader.Unbind();
    delete[] m_positions;
    delete[] m_indices;

    delete m_vertexBuffer;
    delete m_indexBuffer;
}

// http://www.songho.ca/opengl/gl_sphere.html
void Sphere::BuildVerticesSmooth()
{
    const float PI = (float)acos(-1);

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / m_radius;    // normal
    //float s, t;                                     // texCoord

    float sectorStep = 2 * PI / m_sectorCount;
    float stackStep = PI / m_stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= m_stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = m_radius * cosf(stackAngle);             // r * cos(u)
        z = m_radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= m_sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            AddVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            AddNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            /*s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);*/
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < m_stackCount; ++i)
    {
        k1 = i * (m_sectorCount + 1);     // beginning of current stack
        k2 = k1 + m_sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                AddIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
            }

            if (i != (m_stackCount - 1))
            {
                AddIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }
        }
    }

    // generate interleaved vertex array as well
    //BuildInterleavedVertices();
}

void Sphere::AddVertex(float x, float y, float z)
{
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void Sphere::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    m_vec_indices.push_back(i1);
    m_vec_indices.push_back(i2);
    m_vec_indices.push_back(i3);
}

void Sphere::AddNormal(float nx, float ny, float nz)
{
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);
}
