#include "BoundingBox.h"

void BoundingBox::Create(glm::vec3 position, glm::vec3 dimensions) {
	m_position = position;
	m_dimensions = dimensions;
	m_transform = glm::translate(glm::mat4(1.0f), m_position) * glm::scale(glm::mat4(1.0f), m_dimensions);

	// NOT USED
	{
		//// Cube 1x1x1, centered on origin
		//std::vector<glm::vec3> vertices = {
		//  glm::vec3(0.5f, -0.5f, -0.5f),
		//  glm::vec3(-0.5f, -0.5f, -0.5f),
		//  glm::vec3(0.5f,  0.5f, -0.5f),
		//  glm::vec3(-0.5f,  0.5f, -0.5f),
		//  glm::vec3(-0.5f, -0.5f,  0.5f),
		//  glm::vec3(0.5f, -0.5f,  0.5f),
		//  glm::vec3(0.5f,  0.5f,  0.5f),
		//  glm::vec3(-0.5f,  0.5f,  0.5f),
		//};

		//std::vector<unsigned int> indices = {
		//  0, 1, 2, 3,
		//  4, 5, 6, 7,
		//  0, 4, 1, 5,
		//  2, 6, 3, 7
		//};

		//// Use VAO to store state associated with vertices
		//glGenVertexArrays(1, &m_vao);
		//glBindVertexArray(m_vao);

		//// Create a VBO
		//m_vbo.Create();
		//m_vbo.Bind();

		//// Put the vertex attributes in the VBO
		//for (unsigned int i = 0; i < vertices.size(); i++) {
		//	m_vbo.AddVertexData(&vertices[i], sizeof(glm::vec3));
		//}

		//// Put vertex indices into VBO
		//for (unsigned int i = 0; i < indices.size(); i++) {
		//	m_vbo.AddIndexData(&indices[i], sizeof(unsigned int));
		//}

		//m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(
		//	0,
		//	3,
		//	GL_FLOAT,
		//	GL_FALSE,
		//	(GLsizei)sizeof(glm::vec3),
		//	0
		//);
	}
}

// NOT USED
void BoundingBox::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(200);

	glBindVertexArray(m_vao);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(unsigned int)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(unsigned int)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool BoundingBox::intersect(BoundingBox otherBB) {
	glm::vec3 thisMin = minPos();
	glm::vec3 thisMax = maxPos();

	glm::vec3 otherMin = otherBB.minPos();
	glm::vec3 otherMax = otherBB.maxPos();

	return (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
		(thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
		(thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);
}