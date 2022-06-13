#pragma once

#include "Texture.h"
#include "VertexBufferObjectIndexed.h"

// Class for generating a unit octo sphere
class COctaSphere
{
public:
	COctaSphere();
	~COctaSphere();
	void Create(int resolution, int type, int time, bool lightEmitter, string filepath, glm::vec3 position, glm::vec3 scale);
	void Render();
	void Release();

	glm::vec3 GetPosition() const {
		return m_position;
	}

	glm::vec3 GetScale() const {
		return m_scale;
	}

	int GetType() const {
		return m_type;
	}

	int GetTimeOffset() const {
		return m_random_time_offset;
	}

	void SetPosition(glm::vec3 position) {
		m_position = position;
	}

	void SetScale(glm::vec3 scale) {
		m_scale = scale;
	}

	bool UsingTexture() const {
		return m_texture.IsTextureLoaded();
	}

	bool IsLightEmitter() const {
		return m_lightEmitter;
	}

private:
	void CreateOctahedron(std::vector<glm::vec3>& vertices, std::vector<int>& triangles, int resolution);

	int CreateVertexLine(glm::vec3 from, glm::vec3 to, int steps, int v, std::vector<glm::vec3>& vertices);
	int CreateLowerStrip(int steps, int vTop, int vBottom, int t, std::vector<int>& triangles);
	int CreateUpperStrip(int steps, int vTop, int vBottom, int t, std::vector<int>& triangles);

	void Normalize(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);
	void CreateUV(std::vector<glm::vec3>& vertices, std::vector<int>& triangles, std::vector<glm::vec2>& uv);
	void CreateTangents(std::vector<glm::vec3>& vertices, std::vector<glm::vec4>& tangents);

	UINT m_vao;
	CVertexBufferObjectIndexed m_vbo;
	CTexture m_texture;
	string m_filepath;

	glm::vec3 m_position{};
	glm::vec3 m_scale{};
	int m_type = 0; // The type of vertex displacment/colouing to use.
	int m_random_time_offset = 0;
	bool m_lightEmitter = false;

	std::vector<glm::vec3> m_vertices{};
	std::vector<glm::vec3> m_normals{};
	std::vector<glm::vec2> m_uv{};
	std::vector<int> m_triangles{};

	enum Direction {
		LEFT,
		BACK,
		RIGHT,
		FORWARD,
		UP,
		DOWN
	};

	std::vector<glm::vec3> directionVectors{
			glm::vec3(-1,0,0),
			glm::vec3(0,0,1),
			glm::vec3(1,0,0),
			glm::vec3(0,0,-1),
			glm::vec3(0,1,0),
			glm::vec3(0,-1,0)
	};
};