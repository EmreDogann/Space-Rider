#include "Player.h"

void CPlayer::Create(const std::string& filepath, glm::vec3 scale) {
	m_scale = scale;

	m_boundingBox.Create(m_position, glm::vec3(30.0f, 50.0f, 30.0f));

	if (filepath != "") {
		m_Mesh.Load(filepath + "models/Car/model.obj");
		m_emissionTexture.Load(filepath + "models/Car/Emission.png");

		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void CPlayer::Update(float dt, float trackWidth) {
	TranslateByKeyboard(dt);

	m_positionOffset.x = glm::clamp(m_positionOffset.x, -trackWidth / 2.0f, trackWidth / 2.0f);
}

// Respond to keyboard presses on arrow keys to translate the camera
void CPlayer::TranslateByKeyboard(double dt) {
	bool isMoving = false;

	// Accelerate Left
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80 && (m_currentStrafeSpeed == 0 || movingDirection == -1)) {
		Strafe(-1.0 * dt);

		m_currentStrafeSpeed += 0.005f;
		isMoving = true;
		movingDirection = -1;
	}

	// Accelerate Right
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80 && (m_currentStrafeSpeed == 0 || movingDirection == 1)) {
		Strafe(1.0 * dt);

		m_currentStrafeSpeed += 0.005f;
		isMoving = true;
		movingDirection = 1;
	}

	// Decelerate if not pressing movement keys
	if (!isMoving) {
		m_currentStrafeSpeed -= 0.005f;
	}

	m_currentStrafeSpeed = glm::clamp(m_currentStrafeSpeed, 0.0f, m_maxStrafeSpeed);

	// Inertia
	if (!isMoving && m_currentStrafeSpeed != 0) {
		Strafe(movingDirection * dt);
	}
}

// Strafe the camera (move it side to side)
void CPlayer::Strafe(double direction) {
	float speed = (float)(m_currentStrafeSpeed * direction);

	m_positionOffset.x += speed;
}

void CPlayer::Render() {
	glBindVertexArray(m_Mesh.GetVAO());
	m_emissionTexture.Bind(1);
	m_Mesh.Render();
	glBindVertexArray(0);
}