#include "Enemy.h"

void CEnemy::Create(string filename, float distance, glm::vec3 position, glm::vec3 scale) {
	m_distance = distance;
	m_position = position;
	m_scale = scale;

	m_boundingBox.Create(m_position, glm::vec3(30.0f));

	if (filename != "") {
		m_Mesh.Load(filename + "models/drone/drone.obj");
		m_emissionTexture.Load(filename + "models/drone/lambert1_emissive.jpeg");

		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_emissionTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void CEnemy::Update(float dt, float trackWidth) {
	m_currentShootCooldown -= dt / 1000.0f;
}

void CEnemy::Render() {
	glBindVertexArray(m_Mesh.GetVAO());
	m_Mesh.GetTextures()[0]->Bind(0);
	m_emissionTexture.Bind(1);
	m_Mesh.Render();
}