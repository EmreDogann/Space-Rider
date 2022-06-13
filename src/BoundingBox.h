#pragma once

#include "Common.h"
#include "VertexBufferObjectIndexed.h"

class BoundingBox {
public:
	BoundingBox() {};

	~BoundingBox() {
		glDeleteVertexArrays(1, &m_vao);
		m_vbo.Release();
	};

	void Create(glm::vec3 position, glm::vec3 dimensions);

	void Render();

	bool intersect(BoundingBox otherBB);

	void SetPosition(glm::vec3 position) {
		m_position = position;
		m_transform = glm::translate(glm::mat4(1.0f), m_position) * glm::scale(glm::mat4(1.0f), m_dimensions);
	}

	glm::mat4 GetTransform() const {
		return m_transform;
	}

private:
	glm::vec3 minPos() {
		glm::vec3 position{};

		position.x = m_position.x - m_dimensions.x;
		position.y = m_position.y - m_dimensions.y;
		position.z = m_position.z - m_dimensions.z;

		return position;
	}

	glm::vec3 maxPos() {
		glm::vec3 position{};

		position.x = m_position.x + m_dimensions.x;
		position.y = m_position.y + m_dimensions.y;
		position.z = m_position.z + m_dimensions.z;

		return position;
	}

	glm::vec3 m_position{};
	glm::vec3 m_dimensions{};
	glm::mat4 m_transform{};

	UINT m_vao;
	CVertexBufferObjectIndexed m_vbo;
};