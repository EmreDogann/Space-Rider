#pragma once

#include "OpenAssetImportMesh.h"
#include "CatmullRom.h"

class CStreetLight {
public:
	CStreetLight() {};
	~CStreetLight() {};

	void Create(const std::string& filepath, CCatmullRom* track, unsigned int instanceCount);
	void Render();
private:
	COpenAssetImportMesh m_Mesh{};
	unsigned int m_instancedVBO;

	std::vector<glm::mat4> m_modelMatrices;
	unsigned int m_instanceCount = 0;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());

	std::uniform_real_distribution<float> m_random_size = std::uniform_real_distribution<float>(1.0f, 60.0f);
	std::uniform_real_distribution<float> m_random_position = std::uniform_real_distribution<float>(500.0f, 1000.0f);
	std::uniform_real_distribution<float> m_random_rotation = std::uniform_real_distribution<float>(-1.5f, 3.14f);
};