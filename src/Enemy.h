#pragma once

#include "OpenAssetImportMesh.h"
#include "BoundingBox.h"

class CEnemy {
public:
	CEnemy() {};
	~CEnemy() {};

	void Create(string filename, float distance, glm::vec3 position, glm::vec3 scale);

	void Update(float dt, float trackWidth);

	void Render();

	void SetPosition(glm::vec3 newPosition) {
		m_position = newPosition;
		m_boundingBox.SetPosition(newPosition + (m_positionOffset.x * m_strafeVector));
	}

	void SetRotation(glm::mat4 rotation) {
		m_rotation = rotation;
	}

	void SetRotationOffset(glm::mat4 rotation) {
		m_rotationOffset = rotation;
	}

	void SetTNBFrame(glm::vec3 newForward, glm::vec3 newStrafe, glm::vec3 newUp) {
		m_forwardVector = newForward;
		m_strafeVector = newStrafe;
		m_upVector = newUp;
	}

	void SetHealth(float newHealth) {
		m_health = glm::clamp(newHealth, 0.0f, 75.0f);
	}

	void SetDistance(float newDistance) {
		m_distance = newDistance;
	}

	void SetActive(bool active) {
		m_active = active;
	}

	glm::vec3 GetPosition() const {
		return m_position;
	}

	glm::vec3 GetPositionOffset() const {
		return m_positionOffset;
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

	glm::vec3 GetForwardVector() const {
		return m_forwardVector;
	}

	glm::vec3 GetStrafeVector() const {
		return m_strafeVector;
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

	float GetHealth() const {
		return m_health;
	}

	float GetDistance() const {
		return m_distance;
	}

	float GetDamage() const {
		return m_damage;
	}

	bool IsShootCooldownFinished() const {
		return m_currentShootCooldown <= 0.0f;
	}

	void ResetShootCooldown() {
		m_currentShootCooldown = m_shootCooldown;
	}

private:
	COpenAssetImportMesh m_Mesh{};
	CTexture m_emissionTexture;

	glm::vec3 m_position{};
	glm::vec3 m_positionOffset{};
	glm::mat4 m_rotation{};
	glm::mat4 m_rotationOffset{};
	glm::vec3 m_scale{};

	glm::vec3 m_strafeVector{};
	glm::vec3 m_upVector{};
	glm::vec3 m_forwardVector{};

	float m_distance = 0.0f;
	bool m_active = true;

	float m_health = 75.0f;
	float m_damage = 20.0f;
	float m_shootCooldown = 2.0f;
	float m_currentShootCooldown = m_shootCooldown;

	float m_maxStrafeSpeed = 0.15f;
	float m_currentStrafeSpeed = 0.0f;
	int movingVector = 0;

	BoundingBox m_boundingBox;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());

	std::uniform_real_distribution<float> m_random_size = std::uniform_real_distribution<float>(1.0f, 60.0f);
	std::uniform_real_distribution<float> m_random_position = std::uniform_real_distribution<float>(200.0f, 1000.0f);
	std::uniform_real_distribution<float> m_random_rotation = std::uniform_real_distribution<float>(-1.5f, 3.14f);
};