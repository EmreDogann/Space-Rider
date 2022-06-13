#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"
#include "BoundingBox.h"

// Class for generating a Hexagonal Prism
class CHexagon {
public:
	CHexagon();
	~CHexagon();
	void Create(string sFilename, float distance, float value, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));
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

	glm::vec3 GetForwardDirection() const {
		return m_forwardVector;
	}

	glm::vec3 GetStrafeDirection() const {
		return m_strafeVector;
	}

	glm::vec3 GetUpDirection() const {
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

	float GetValue() const {
		return m_value;
	}

	int GetTriggeredBy() const {
		return m_triggeredBy;
	}

	void SetTNBFrame(glm::vec3 newForward, glm::vec3 newStrafe, glm::vec3 newUp) {
		m_forwardVector = newForward;
		m_strafeVector = newStrafe;
		m_upVector = newUp;
	}

	void SetPosition(glm::vec3 position) {
		m_position = position;
		m_boundingBox.SetPosition(position);
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

	void SetTriggeredBy(float triggeredBy) {
		m_triggeredBy = triggeredBy;
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

	glm::vec3 m_strafeVector{};
	glm::vec3 m_upVector{};
	glm::vec3 m_forwardVector{};

	float m_value = 10.0f;
	float m_distance = 0.0f;
	bool m_active = true;
	int m_triggeredBy;

	BoundingBox m_boundingBox;
};