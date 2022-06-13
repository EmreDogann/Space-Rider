#include "Common.h"
#include "Health.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

CHealth::CHealth()
{}

CHealth::~CHealth()
{}

// Create the health plus sign, including its geometry, texture mapping, normal, and colour
void CHealth::Create(string filename, float distance, glm::vec3 position, glm::vec3 scale) {
	m_distance = distance;
	m_position = position;
	m_scale = scale;

	m_boundingBox.Create(position, glm::vec3(20.0f));

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
	std::vector<glm::vec3> healthVertices = {
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -3.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 3.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),

		glm::vec3(-1.0f, 3.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, 1.0f),
		glm::vec3(1.0f, 3.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, -1.0f, -3.0f),
		glm::vec3(-1.0f, -1.0f, -3.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -3.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		glm::vec3(1.0f, -3.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, 1.0f),
		glm::vec3(-1.0f, -3.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -3.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),

		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		glm::vec3(1.0f, 1.0f, 3.0f),
		glm::vec3(-1.0f, -1.0f, 3.0f),
		glm::vec3(1.0f, -1.0f, 3.0f),

		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),

		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 3.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -3.0f),
		glm::vec3(-1.0f, 1.0f, -3.0f),

		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 3.0f),
		glm::vec3(1.0f, 1.0f, 3.0f),

		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 3.0f, 1.0f),
		glm::vec3(-1.0f, 3.0f, 1.0f),

		glm::vec3(-1.0f, 3.0f, -1.0f),
		glm::vec3(-1.0f, 3.0f, 1.0f),
		glm::vec3(1.0f, 3.0f, 1.0f),

		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 3.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, -1.0f, -3.0f),

		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 3.0f, 1.0f),
		glm::vec3(-1.0f, 3.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -3.0f),
		glm::vec3(-1.0f, -1.0f, -3.0f),

		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 3.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, 3.0f),
		glm::vec3(-1.0f, 1.0f, 3.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),

		glm::vec3(-1.0f, -3.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 3.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),

		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -3.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -3.0f),
		glm::vec3(1.0f, -1.0f, -3.0f),

		glm::vec3(1.0f, -3.0f, -1.0f),
		glm::vec3(1.0f, -3.0f, 1.0f),
		glm::vec3(-1.0f, -3.0f, 1.0f),

		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -3.0f, 1.0f),
		glm::vec3(1.0f, -3.0f, 1.0f),

		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -3.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, -1.0f),

		glm::vec3(1.0f, 1.0f, 3.0f),
		glm::vec3(-1.0f, 1.0f, 3.0f),
		glm::vec3(-1.0f, -1.0f, 3.0f),

		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, 1.0f),

		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 3.0f),
		glm::vec3(1.0f, -1.0f, 3.0f),

		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 3.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(1.0f, -1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),

		glm::vec3(1.0f, -3.0f, 1.0f),
		glm::vec3(1.0f, -3.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),

		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),

		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 3.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),

		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -3.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 3.0f),
		glm::vec3(-1.0f, -1.0f, 3.0f),
	};

	// Texture Coordinates
	std::vector<glm::vec2> healthTexCoords = {
		glm::vec2(0.31f, 0.08f),
		glm::vec2(0.23f, 0.15f),
		glm::vec2(0.23f, 0.08f),

		glm::vec2(0.23f, 0.77f),
		glm::vec2(0.31f, 0.69f),
		glm::vec2(0.31f, 0.77f),

		glm::vec2(0.31f, 0.77f),
		glm::vec2(0.23f, 0.85f),
		glm::vec2(0.23f, 0.77f),

		glm::vec2(0.23f, 0.92f),
		glm::vec2(0.31f, 0.85f),
		glm::vec2(0.31f, 0.92f),

		glm::vec2(0.23f, 0.08f),
		glm::vec2(0.31f, 0.0f),
		glm::vec2(0.31f, 0.08f),

		glm::vec2(0.23f, 0.15f),
		glm::vec2(0.31f, 0.23f),
		glm::vec2(0.23f, 0.23f),

		glm::vec2(0.08f, 0.92f),
		glm::vec2(0.08f, 0.85f),
		glm::vec2(0.15f, 0.85f),

		glm::vec2(0.23f, 0.31f),
		glm::vec2(0.31f, 0.23f),
		glm::vec2(0.31f, 0.31f),

		glm::vec2(0.31f, 0.38f),
		glm::vec2(0.23f, 0.46f),
		glm::vec2(0.23f, 0.38f),

		glm::vec2(0.23f, 0.54f),
		glm::vec2(0.31f, 0.46f),
		glm::vec2(0.31f, 0.54f),

		glm::vec2(0.31f, 0.31f),
		glm::vec2(0.23f, 0.38f),
		glm::vec2(0.23f, 0.31f),

		glm::vec2(0.31f, 0.69f),
		glm::vec2(0.23f, 0.62f),
		glm::vec2(0.31f, 0.62f),

		glm::vec2(0.46f, 0.85f),
		glm::vec2(0.46f, 0.92f),
		glm::vec2(0.38f, 0.92f),

		glm::vec2(0.31f, 0.54f),
		glm::vec2(0.23f, 0.62f),
		glm::vec2(0.23f, 0.54f),

		glm::vec2(0.31f, 0.08f),
		glm::vec2(0.31f, 0.15f),
		glm::vec2(0.23f, 0.15f),

		glm::vec2(0.23f, 0.77f),
		glm::vec2(0.23f, 0.69f),
		glm::vec2(0.31f, 0.69f),

		glm::vec2(0.31f, 0.77f),
		glm::vec2(0.31f, 0.85f),
		glm::vec2(0.23f, 0.85f),

		glm::vec2(0.23f, 0.92f),
		glm::vec2(0.23f, 0.85f),
		glm::vec2(0.31f, 0.85f),

		glm::vec2(0.23f, 0.08f),
		glm::vec2(0.23f, 0.0f),
		glm::vec2(0.31f, 0.0f),

		glm::vec2(0.23f, 0.15f),
		glm::vec2(0.31f, 0.15f),
		glm::vec2(0.31f, 0.23f),

		glm::vec2(0.15f, 0.85f),
		glm::vec2(0.23f, 0.85f),
		glm::vec2(0.23f, 0.92f),

		glm::vec2(0.15f, 0.92f),
		glm::vec2(0.15f, 1.0f),
		glm::vec2(0.08f, 1.0f),

		glm::vec2(0.15f, 0.85f),
		glm::vec2(0.23f, 0.92f),
		glm::vec2(0.15f, 0.92f),

		glm::vec2(0.08f, 0.77f),
		glm::vec2(0.15f, 0.77f),
		glm::vec2(0.15f, 0.85f),

		glm::vec2(0.0f, 0.92f),
		glm::vec2(0.0f, 0.85f),
		glm::vec2(0.08f, 0.85f),

		glm::vec2(0.08f, 0.85f),
		glm::vec2(0.08f, 0.77f),
		glm::vec2(0.15f, 0.85f),

		glm::vec2(0.15f, 0.85f),
		glm::vec2(0.15f, 0.92f),
		glm::vec2(0.08f, 0.92f),

		glm::vec2(0.15f, 0.92f),
		glm::vec2(0.08f, 1.0f),
		glm::vec2(0.08f, 0.92f),

		glm::vec2(0.08f, 0.92f),
		glm::vec2(0.0f, 0.92f),
		glm::vec2(0.08f, 0.85f),

		glm::vec2(0.23f, 0.31f),
		glm::vec2(0.23f, 0.23f),
		glm::vec2(0.31f, 0.23f),

		glm::vec2(0.31f, 0.38f),
		glm::vec2(0.31f, 0.46f),
		glm::vec2(0.23f, 0.46f),

		glm::vec2(0.23f, 0.54f),
		glm::vec2(0.23f, 0.46f),
		glm::vec2(0.31f, 0.46f),

		glm::vec2(0.31f, 0.31f),
		glm::vec2(0.31f, 0.38f),
		glm::vec2(0.23f, 0.38f),

		glm::vec2(0.31f, 0.69f),
		glm::vec2(0.23f, 0.69f),
		glm::vec2(0.23f, 0.62f),

		glm::vec2(0.38f, 0.92f),
		glm::vec2(0.31f, 0.92f),
		glm::vec2(0.31f, 0.85f),

		glm::vec2(0.38f, 0.85f),
		glm::vec2(0.38f, 0.77f),
		glm::vec2(0.46f, 0.77f),

		glm::vec2(0.38f, 0.92f),
		glm::vec2(0.31f, 0.85f),
		glm::vec2(0.38f, 0.85f),

		glm::vec2(0.46f, 1.0f),
		glm::vec2(0.38f, 1.0f),
		glm::vec2(0.38f, 0.92f),

		glm::vec2(0.54f, 0.85f),
		glm::vec2(0.54f, 0.92f),
		glm::vec2(0.46f, 0.92f),

		glm::vec2(0.46f, 0.92f),
		glm::vec2(0.46f, 1.0f),
		glm::vec2(0.38f, 0.92f),

		glm::vec2(0.38f, 0.92f),
		glm::vec2(0.38f, 0.85f),
		glm::vec2(0.46f, 0.85f),

		glm::vec2(0.38f, 0.85f),
		glm::vec2(0.46f, 0.77f),
		glm::vec2(0.46f, 0.85f),

		glm::vec2(0.46f, 0.85f),
		glm::vec2(0.54f, 0.85f),
		glm::vec2(0.46f, 0.92f),

		glm::vec2(0.31f, 0.54f),
		glm::vec2(0.31f, 0.62f),
		glm::vec2(0.23f, 0.62f),
	};

	// Normals Vectors
	std::vector<glm::vec3> healthNormal = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
	};

	// Vertex Indices
	std::vector<unsigned int> healthIndices = {
		0,1,2,			3,4,5,
		6,7,8,			9,10,11,
		12,13,14,       15,16,17,
		18,19,20,       21,22,23,
		24,25,26,       27,28,29,
		30,31,32,       33,34,35,
		36,37,38,       39,40,41,
		42,43,44,       45,46,47,
		48,49,50,       51,52,53,
		54,55,56,       57,58,59,
		60,61,62,       63,64,65,
		66,67,68,       69,70,71,
		72,73,74,       75,76,77,
		78,79,80,       81,82,83,
		84,85,86,       87,88,89,
		90,91,92,       93,94,95,
		96,97,98,       99,100,101,
		102,103,104,    105,106,107,
		108,109,110,    111,112,113,
		114,115,116,    117,118,119,
		120,121,122,    123,124,125,
		126,127,128,    129,130,131,
	};

	// Put the vertex attributes in the VBO
	for (unsigned int i = 0; i < healthVertices.size(); i++) {
		m_vbo.AddVertexData(&healthVertices[i], sizeof(glm::vec3));
		m_vbo.AddVertexData(&healthTexCoords[i], sizeof(glm::vec2));
		m_vbo.AddVertexData(&healthNormal[i], sizeof(glm::vec3));
	}

	// Put vertex indices into VBO
	for (unsigned int i = 0; i < healthIndices.size(); i++) {
		m_vbo.AddIndexData(&healthIndices[i], sizeof(unsigned int));
	}

	m_indexCount = healthIndices.size();

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

// Render the health sign as triangles
void CHealth::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

// Release resources
void CHealth::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}