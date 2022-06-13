#include "Rock.h"

void CRock::Create(const std::string& filepath, unsigned int instanceCount) {
	m_instanceCount = instanceCount;

	m_Mesh.Load(filepath + "models/asteroid1/model.obj");
	m_MeshAlt.Load(filepath + "models/asteroid2/model.obj");

	m_modelMatrices.resize(instanceCount / 2, glm::mat4(1.0f));
	m_modelMatricesAlt.resize(instanceCount / 2, glm::mat4(1.0f));

	for (int i = 0; i < instanceCount / 2; ++i) {
		glm::mat4 modelMatrix(1.0f);

		// Create random model matrices.
		modelMatrix = glm::translate(modelMatrix, glm::vec3(m_random_position(mt), m_random_position(mt), m_random_position(mt)));
		modelMatrix = glm::rotate(modelMatrix, m_random_rotation(mt), glm::vec3(0.4f, 0.6f, 0.8f));
		float scale = m_random_size(mt);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

		m_modelMatrices[i] = modelMatrix;

		// Create random model matrices for alternate mesh.
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(m_random_position(mt), m_random_position(mt), m_random_position(mt)));
		modelMatrix = glm::rotate(modelMatrix, m_random_rotation(mt), glm::vec3(0.4f, 0.6f, 0.8f));
		scale = m_random_size(mt);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

		m_modelMatricesAlt[i] = modelMatrix;
	}

	glBindVertexArray(m_Mesh.GetVAO());

	// Vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_Mesh.GetEntries()[0].vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	// Vertex buffer object
	glGenBuffers(1, &m_instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, (instanceCount / 2) * sizeof(glm::mat4), &m_modelMatrices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	std::size_t vec4Size = sizeof(glm::vec4);
	// We need to reserve a vertex attribute slot for a mat4 which is the instance's mode matrix.
	// However, vertex attributes can only store a max of 1 vec4. A mat4 is 4 x vec4. So, we need to reserve 4 vertex attribute slots for 1 mat4.
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	// Tells OpenGL to only update these vertex attribute fields when rendering a new instance.
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	// Setup vertex attributes for alternate mesh
	glBindVertexArray(m_MeshAlt.GetVAO());

	// Vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshAlt.GetEntries()[0].vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	// Vertex buffer object
	glGenBuffers(1, &m_instancedVBOAlt);
	glBindBuffer(GL_ARRAY_BUFFER, m_instancedVBOAlt);
	glBufferData(GL_ARRAY_BUFFER, (instanceCount / 2) * sizeof(glm::mat4), &m_modelMatricesAlt[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	// We need to reserve a vertex attribute slot for a mat4 which is the instance's mode matrix.
	// However, vertex attributes can only store a max of 1 vec4. A mat4 is 4 x vec4. So, we need to reserve 4 vertex attribute slots for 1 mat4.
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	// Tells OpenGL to only update these vertex attribute fields when rendering a new instance.
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void CRock::Render() {
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Draw mesh
	glBindVertexArray(m_Mesh.GetVAO());
	m_Mesh.GetTextures()[0]->Bind(0);

	glDrawElementsInstanced(GL_TRIANGLES, m_Mesh.GetEntries()[0].NumIndices, GL_UNSIGNED_INT, 0, m_instanceCount / 2);

	// Draw alternate mesh
	glBindVertexArray(m_MeshAlt.GetVAO());
	m_Mesh.GetTextures()[0]->Bind(0);

	glDrawElementsInstanced(GL_TRIANGLES, m_MeshAlt.GetEntries()[0].NumIndices, GL_UNSIGNED_INT, 0, m_instanceCount / 2);
	glBindVertexArray(0);
}