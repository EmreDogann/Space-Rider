#include "StreetLight.h"

void CStreetLight::Create(const std::string& filepath, CCatmullRom* track, unsigned int instanceCount) {
	m_Mesh.Load(filepath + "models/streetLight/model.obj");

	float distance = 0.0f;
	bool trackSideLeft = true;
	for (int i = 0; i < instanceCount; ++i) {
		glm::vec3 p;
		track->Sample(distance, p);

		// Look ahead a bit.
		glm::vec3 pNext;
		track->Sample(distance + 1.0f, pNext);

		// Tangent Vector
		glm::vec3 vecT = glm::normalize(pNext - p);
		glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
		glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector

		glm::vec3 finalPosition{};
		glm::mat4 lookDirection{};
		// Place street light on left of right side of track, looking towards the centerline.
		if (trackSideLeft) {
			finalPosition = p - (track->GetTrackWidth() / 2) * vecN;
			finalPosition -= 10.0f * vecB;
			lookDirection = glm::inverse(glm::lookAt(finalPosition, finalPosition + vecT, glm::vec3(0.0f, 1.0f, 0.0f)));
		}
		else {
			finalPosition = p + (track->GetTrackWidth() / 2) * vecN;
			finalPosition -= 10.0f * vecB;
			lookDirection = glm::inverse(glm::lookAt(finalPosition, finalPosition - vecT, glm::vec3(0.0f, 1.0f, 0.0f)));
		}
		trackSideLeft = !trackSideLeft; // Alternate between left and right.

		glm::mat4 modelMatrix(1.0f);
		modelMatrix *= lookDirection;
		//float scale = m_random_size(mt);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));

		m_modelMatrices.push_back(modelMatrix);

		distance += m_random_position(mt);

		// Don't spawn anymore street lamps if the end/start of the track has been reached.
		if (track->CurrentLap(distance) > 0) {
			m_instanceCount = i;
			break;
		}
		else {
			m_instanceCount = instanceCount;
		}
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
	glBufferData(GL_ARRAY_BUFFER, m_instanceCount * sizeof(glm::mat4), &m_modelMatrices[0], GL_STATIC_DRAW);

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

	glBindVertexArray(0);
}

void CStreetLight::Render() {
	glBindVertexArray(m_Mesh.GetVAO());
	for (int i = 0; i < m_Mesh.GetTextures().size(); ++i) {
		m_Mesh.GetTextures()[i]->Bind(i);
	}

	glDrawElementsInstanced(GL_TRIANGLES, m_Mesh.GetEntries()[0].NumIndices, GL_UNSIGNED_INT, 0, m_instanceCount);
	glBindVertexArray(0);
}