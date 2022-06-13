#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"
#include "BoundingBox.h"

// Class for generating a Health plus sign
class CHealth {
public:
	CHealth();
	~CHealth();
	void Create(string sFilename, float distance, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
	void Render();
	void Release();

	glm::vec3 GetPosition() const {
		return m_position;
	}

	glm::mat4 GetRotation() const {
		return m_rotation;
	}

	glm::mat4 GetRotationOffset() const {
		return m_rotationOffset;
	}

	glm::vec3 GetScale() const {
		return m_scale;
	}

	glm::vec3 GetUpVector() const {
		return m_upVector;
	}

	bool GetActive() const {
		return m_active;
	}

	BoundingBox GetBoundingBox() const {
		return m_boundingBox;
	}

	float GetDistance() const {
		return m_distance;
	}

	float GetHealth() const {
		return m_health;
	}

	void SetPosition(glm::vec3 position) {
		m_position = position;
	}

	void SetRotation(glm::mat4 rotation) {
		m_rotation = rotation;
	}

	void SetRotationOffset(glm::mat4 rotation) {
		m_rotationOffset = rotation;
	}

	void SetScale(glm::vec3 scale) {
		m_scale = scale;
	}

	void SetUpVector(glm::vec3 upVector) {
		m_upVector = upVector;
	}

	void SetActive(bool active) {
		m_active = active;
	}

	void SetDistance(float newDistance) {
		m_distance = newDistance;
	}

private:
	UINT m_vao;
	CVertexBufferObjectIndexed m_vbo;
	CTexture m_texture;
	string m_filename;

	unsigned int m_indexCount = 0;

	glm::vec3 m_position{};
	glm::mat4 m_rotation{};
	glm::vec3 m_scale{};
	glm::mat4 m_rotationOffset{};
	glm::vec3 m_upVector{};

	float m_health = 25.0f;
	float m_distance = 0.0f;
	bool m_active = true;
	BoundingBox m_boundingBox;
};