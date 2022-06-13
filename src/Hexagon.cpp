#include "Common.h"
#include "Hexagon.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

CHexagon::CHexagon()
{}

CHexagon::~CHexagon()
{}

// Create the hexagonal prism, including its geometry, texture mapping, normal, and colour
void CHexagon::Create(string filename, float distance, float value, glm::vec3 position, glm::vec3 scale) {
	m_distance = distance;
	m_position = position;
	m_scale = scale;
	m_value = value;

	m_boundingBox.Create(position, glm::vec3(15.0f));

	// Load the texture
	if (filename != "") {
		m_texture.Load(filename, true);
		m_filename = filename;

		// Set parameters for texturing using sampler object
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create a VBO
	m_vbo.Create();
	m_vbo.Bind();

	// Vertex Positions
	std::vector<glm::vec3> hexVertices = {
		glm::vec3(0.0f, 2.0f, -1.0f),
		glm::vec3(0.87f, 0.0f, -0.5f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.87f, 2.0f, -0.5f),
		glm::vec3(0.87f, 0.0f, 0.5f),
		glm::vec3(0.87f, 0.0f, -0.5f),

		glm::vec3(0.87f, 2.0f, 0.5f),
		glm::vec3(-0.0f, 0.0f, 1.0f),
		glm::vec3(0.87f, 0.0f, 0.5f),

		glm::vec3(-0.0f, 2.0f, 1.0f),
		glm::vec3(-0.87f, 0.0f, 0.5f),
		glm::vec3(-0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 2.0f, -1.0f),
		glm::vec3(-0.87f, 2.0f, 0.5f),
		glm::vec3(0.87f, 2.0f, 0.5f),

		glm::vec3(-0.87f, 2.0f, 0.5f),
		glm::vec3(-0.87f, 0.0f, -0.5f),
		glm::vec3(-0.87f, 0.0f, 0.5f),

		glm::vec3(-0.87f, 2.0f, -0.5f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-0.87f, 0.0f, -0.5f),

		glm::vec3(-0.87f, 0.0f, 0.5f),
		glm::vec3(-0.87f, 0.0f, -0.5f),
		glm::vec3(0.87f, 0.0f, -0.5f),

		glm::vec3(0.0f, 2.0f, -1.0f),
		glm::vec3(0.87f, 2.0f, -0.5f),
		glm::vec3(0.87f, 0.0f, -0.5f),

		glm::vec3(0.87f, 2.0f, -0.5f),
		glm::vec3(0.87f, 2.0f, 0.5f),
		glm::vec3(0.87f, 0.0f, 0.5f),

		glm::vec3(0.87f, 2.0f, 0.5f),
		glm::vec3(-0.0f, 2.0f, 1.0f),
		glm::vec3(-0.0f, 0.0f, 1.0f),

		glm::vec3(-0.0f, 2.0f, 1.0f),
		glm::vec3(-0.87f, 2.0f, 0.5f),
		glm::vec3(-0.87f, 0.0f, 0.5f),

		glm::vec3(0.87f, 2.0f, 0.5f),
		glm::vec3(0.87f, 2.0f, -0.5f),
		glm::vec3(0.0f, 2.0f, -1.0f),

		glm::vec3(0.0f, 2.0f, -1.0f),
		glm::vec3(-0.87f, 2.0f, -0.5f),
		glm::vec3(-0.87f, 2.0f, 0.5f),

		glm::vec3(-0.87f, 2.0f, 0.5f),
		glm::vec3(-0.0f, 2.0f, 1.0f),
		glm::vec3(0.87f, 2.0f, 0.5f),

		glm::vec3(-0.87f, 2.0f, 0.5f),
		glm::vec3(-0.87f, 2.0f, -0.5f),
		glm::vec3(-0.87f, 0.0f, -0.5f),

		glm::vec3(-0.87f, 2.0f, -0.5f),
		glm::vec3(0.0f, 2.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(-0.87f, 0.0f, -0.5f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.87f, 0.0f, -0.5f),

		glm::vec3(0.87f, 0.0f, -0.5f),
		glm::vec3(0.87f, 0.0f, 0.5f),
		glm::vec3(-0.0f, 0.0f, 1.0f),

		glm::vec3(-0.0f, 0.0f, 1.0f),
		glm::vec3(-0.87f, 0.0f, 0.5f),
		glm::vec3(0.87f, 0.0f, -0.5f),
	};

	// Texture Coordinates
	std::vector<glm::vec2> hexTexCoords = {
		glm::vec2(0.27f, 0.38f),
		glm::vec2(0.57f, 0.23f),
		glm::vec2(0.57f, 0.38f),

		glm::vec2(0.27f, 0.23f),
		glm::vec2(0.57f, 0.08f),
		glm::vec2(0.57f, 0.23f),

		glm::vec2(0.27f, 1.0f),
		glm::vec2(0.57f, 0.85f),
		glm::vec2(0.57f, 1.0f),

		glm::vec2(0.27f, 0.85f),
		glm::vec2(0.57f, 0.69f),
		glm::vec2(0.57f, 0.85f),

		glm::vec2(0.13f, 0.31f),
		glm::vec2(0.0f, 0.08f),
		glm::vec2(0.27f, 0.08f),

		glm::vec2(0.27f, 0.69f),
		glm::vec2(0.57f, 0.54f),
		glm::vec2(0.57f, 0.69f),

		glm::vec2(0.27f, 0.54f),
		glm::vec2(0.57f, 0.38f),
		glm::vec2(0.57f, 0.54f),

		glm::vec2(0.84f, 0.08f),
		glm::vec2(0.84f, 0.23f),
		glm::vec2(0.57f, 0.23f),

		glm::vec2(0.27f, 0.38f),
		glm::vec2(0.27f, 0.23f),
		glm::vec2(0.57f, 0.23f),

		glm::vec2(0.27f, 0.23f),
		glm::vec2(0.27f, 0.08f),
		glm::vec2(0.57f, 0.08f),

		glm::vec2(0.27f, 1.0f),
		glm::vec2(0.27f, 0.85f),
		glm::vec2(0.57f, 0.85f),

		glm::vec2(0.27f, 0.85f),
		glm::vec2(0.27f, 0.69f),
		glm::vec2(0.57f, 0.69f),

		glm::vec2(0.27f, 0.08f),
		glm::vec2(0.27f, 0.23f),
		glm::vec2(0.13f, 0.31f),

		glm::vec2(0.13f, 0.31f),
		glm::vec2(0.0f, 0.23f),
		glm::vec2(0.0f, 0.08f),

		glm::vec2(0.0f, 0.08f),
		glm::vec2(0.13f, 0.0f),
		glm::vec2(0.27f, 0.08f),

		glm::vec2(0.27f, 0.69f),
		glm::vec2(0.27f, 0.54f),
		glm::vec2(0.57f, 0.54f),

		glm::vec2(0.27f, 0.54f),
		glm::vec2(0.27f, 0.38f),
		glm::vec2(0.57f, 0.38f),

		glm::vec2(0.84f, 0.23f),
		glm::vec2(0.71f, 0.31f),
		glm::vec2(0.57f, 0.23f),

		glm::vec2(0.57f, 0.23f),
		glm::vec2(0.57f, 0.08f),
		glm::vec2(0.71f, 0.0f),

		glm::vec2(0.71f, 0.0f),
		glm::vec2(0.84f, 0.08f),
		glm::vec2(0.57f, 0.23f),
	};

	// Normals Vectors
	std::vector<glm::vec3> hexNormal = {
		glm::vec3(0.5f, 0.0f, -0.87f),
		glm::vec3(0.5f, 0.0f, -0.87f),
		glm::vec3(0.5f, 0.0f, -0.87f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.0f, 0.87f),
		glm::vec3(0.5f, 0.0f, 0.87f),
		glm::vec3(0.5f, 0.0f, 0.87f),

		glm::vec3(-0.5f, 0.0f, 0.87f),
		glm::vec3(-0.5f, 0.0f, 0.87f),
		glm::vec3(-0.5f, 0.0f, 0.87f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, 0.0f, -0.87f),
		glm::vec3(-0.5f, 0.0f, -0.87f),
		glm::vec3(-0.5f, 0.0f, -0.87f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.5f, 0.0f, -0.87f),
		glm::vec3(0.5f, 0.0f, -0.87f),
		glm::vec3(0.5f, 0.0f, -0.87f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.0f, 0.87f),
		glm::vec3(0.5f, 0.0f, 0.87f),
		glm::vec3(0.5f, 0.0f, 0.87f),

		glm::vec3(-0.5f, 0.0f, 0.87f),
		glm::vec3(-0.5f, 0.0f, 0.87f),
		glm::vec3(-0.5f, 0.0f, 0.87f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, 0.0f, -0.87f),
		glm::vec3(-0.5f, 0.0f, -0.87f),
		glm::vec3(-0.5f, 0.0f, -0.87f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
	};

	// Vertex Indices
	std::vector<unsigned int> hexIndices = {
		0,1,2,		3,4,5,
		6,7,8,		9,10,11,
		12,13,14,   15,16,17,
		18,19,20,   21,22,23,
		24,25,26,   27,28,29,
		30,31,32,   33,34,35,
		36,37,38,   39,40,41,
		42,43,44,   45,46,47,
		48,49,50,   51,52,53,
		54,55,56,   57,58,59,
	};

	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < hexVertices.size(); i++) {
		m_vbo.AddVertexData(&hexVertices[i], sizeof(glm::vec3));
		m_vbo.AddVertexData(&hexTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddVertexData(&hexNormal[i], sizeof(glm::vec3));
	}

	// Put vertex indices into VBO
	for (unsigned int i = 0; i < hexIndices.size(); i++) {
		m_vbo.AddIndexData(&hexIndices[i], sizeof(unsigned int));
	}

	m_indexCount = hexIndices.size();

	// Upload the VBO to the GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei istride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, istride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}

// Render the prism as triangles
void CHexagon::Render() {
	glBindVertexArray(m_vao);

	if (m_texture.IsTextureLoaded()) {
		m_texture.Bind();
	}

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

// Release resources
void CHexagon::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}