#pragma once

#include "OpenAssetImportMesh.h"

class CRock {
public:
	CRock() {};
	~CRock() {};

	void Create(const std::string& filepath, unsigned int instanceCount);
	void Render();
private:
	COpenAssetImportMesh m_Mesh{};
	COpenAssetImportMesh m_MeshAlt{};
	unsigned int m_instancedVBO;
	unsigned int m_instancedVBOAlt;

	std::vector<glm::mat4> m_modelMatrices;
	std::vector<glm::mat4> m_modelMatricesAlt;
	unsigned int m_instanceCount = 0;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());

	std::uniform_real_distribution<float> m_random_size = std::uniform_real_distribution<float>(1.0f, 60.0f);
	std::uniform_real_distribution<float> m_random_position = std::uniform_real_distribution<float>(-10000.0f, 10000.0f);
	std::uniform_real_distribution<float> m_random_rotation = std::uniform_real_distribution<float>(-1.5f, 3.14f);
};