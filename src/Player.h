#pragma once

#include "OpenAssetImportMesh.h"
#include "BoundingBox.h"

class CPlayer {
public:
	CPlayer() {};
	~CPlayer() {};

	void Create(const std::string& filepath, glm::vec3 scale);

	void Update(float dt, float trackWidth);

	void Render();

	// Respond to keyboard presses on arrow keys to translate the camera
	void TranslateByKeyboard(double dt);

	// Strafe the camera (move it side to side)
	void Strafe(double direction);

	void SetPosition(glm::vec3 newPosition) {
		m_position = newPosition;
		m_boundingBox.SetPosition((newPosition + 50.0f * m_forwardDirection) + (m_positionOffset.x * m_strafeDirection));
	}

	void SetTNBFrame(glm::vec3 newForward, glm::vec3 newStrafe, glm::vec3 newUp) {
		m_forwardDirection = newForward;
		m_strafeDirection = newStrafe;
		m_upDirection = newUp;
	}

	void SetHealth(float newHealth) {
		m_health = glm::clamp(newHealth, 0.0f, m_maxHealth);
	}

	glm::vec3 GetPosition() const {
		return m_position;
	}

	glm::vec3 GetScale() const {
		return m_scale;
	}

	glm::vec3 GetPositionOffset() const {
		return m_positionOffset;
	}

	glm::vec3 GetForwardDirection() const {
		return m_forwardDirection;
	}

	glm::vec3 GetStrafeDirection() const {
		return m_strafeDirection;
	}

	glm::vec3 GetUpDirection() const {
		return m_upDirection;
	}

	BoundingBox GetBoundingBox() const {
		return m_boundingBox;
	}

	float GetHealth() const {
		return m_health;
	}

	float GetMaxHealth() const {
		return m_maxHealth;
	}

private:
	COpenAssetImportMesh m_Mesh{};
	CTexture m_emissionTexture;

	glm::vec3 m_position{};
	glm::vec3 m_positionOffset{};
	glm::vec3 m_scale{};

	float m_maxStrafeSpeed = 0.25f;
	float m_currentStrafeSpeed = 0.0f;
	int movingDirection = 0;

	float m_maxHealth = 100.0f;
	float m_health = 75.0f;

	glm::vec3 m_strafeDirection{};
	glm::vec3 m_upDirection{};
	glm::vec3 m_forwardDirection{};

	BoundingBox m_boundingBox;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());

	std::uniform_real_distribution<float> m_random_size = std::uniform_real_distribution<float>(1.0f, 60.0f);
	std::uniform_real_distribution<float> m_random_position = std::uniform_real_distribution<float>(200.0f, 1000.0f);
	std::uniform_real_distribution<float> m_random_rotation = std::uniform_real_distribution<float>(-1.5f, 3.14f);
};