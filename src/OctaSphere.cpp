#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "OctaSphere.h"
#include <math.h>
#include <unordered_map>

COctaSphere::COctaSphere()
{}

COctaSphere::~COctaSphere()
{}

// Create a unit octa sphere
void COctaSphere::Create(int subDivisions, int type, int time, bool lightEmitter, string filepath, glm::vec3 position, glm::vec3 scale) {
	m_type = type;
	m_random_time_offset = time;
	m_position = position;
	m_scale = scale;
	m_lightEmitter = lightEmitter;

	if (filepath != "") {
		m_texture.Load(filepath);

		m_filepath = filepath;

		m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	int resolution = 1 << subDivisions;
	m_vertices.resize((resolution + 1) * (resolution + 1) * 4 - (resolution * 2 - 1) * 3);
	m_triangles.resize((1 << (subDivisions * 2 + 3)) * 3);

	CreateOctahedron(m_vertices, m_triangles, resolution);

	m_uv.resize(m_vertices.size());
	CreateUV(m_vertices, m_triangles, m_uv);

	m_normals.resize(m_vertices.size());
	Normalize(m_vertices, m_normals);

	//std::vector<glm::vec4> tangents(m_vertices.size());
	//CreateTangents(m_vertices, tangents);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_vbo.Create();
	m_vbo.Bind();

	for (int i = 0; i < m_vertices.size(); ++i) {
		m_vbo.AddVertexData(&m_vertices[i], sizeof(glm::vec3));
		m_vbo.AddVertexData(&m_uv[i], sizeof(glm::vec2));
		m_vbo.AddVertexData(&m_normals[i], sizeof(glm::vec3));
	}

	// Compute indices and store in VBO
	for (int i = 0; i < m_triangles.size(); ++i) {
		m_vbo.AddIndexData(&m_triangles[i], sizeof(unsigned int));
	}

	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void COctaSphere::CreateOctahedron(std::vector<glm::vec3>& vertices, std::vector<int>& triangles, int resolution) {
	int v = 0, vBottom = 0, t = 0;

	for (int i = 0; i < 4; i++) {
		vertices[v++] = directionVectors[DOWN];
	}

	for (int i = 1; i <= resolution; i++) {
		float progress = (float)i / resolution;
		glm::vec3 from{}, to{};

		vertices[v++] = to = glm::slerp(directionVectors[DOWN], directionVectors[FORWARD], progress);
		for (int d = 0; d < 4; d++) {
			from = to;
			to = glm::slerp(directionVectors[DOWN], directionVectors[d], progress);
			t = CreateLowerStrip(i, v, vBottom, t, triangles);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i > 1 ? (i - 1) : 1;
		}
		vBottom = v - 1 - i * 4;
	}

	for (int i = resolution - 1; i >= 1; i--) {
		float progress = (float)i / resolution;
		glm::vec3 from{}, to{};
		vertices[v++] = to = glm::slerp(directionVectors[UP], directionVectors[FORWARD], progress);
		for (int d = 0; d < 4; d++) {
			from = to;
			to = glm::slerp(directionVectors[UP], directionVectors[d], progress);
			t = CreateUpperStrip(i, v, vBottom, t, triangles);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i + 1;
		}
		vBottom = v - 1 - i * 4;
	}

	for (int i = 0; i < 4; i++) {
		triangles[t++] = vBottom;
		triangles[t++] = v;
		triangles[t++] = ++vBottom;
		vertices[v++] = directionVectors[UP];
	}
}

int COctaSphere::CreateVertexLine(glm::vec3 from, glm::vec3 to, int steps, int v, std::vector<glm::vec3>& vertices) {
	for (int i = 1; i <= steps; i++) {
		vertices[v++] = glm::slerp(from, to, (float)i / steps);
	}
	return v;
}

int COctaSphere::CreateLowerStrip(int steps, int vTop, int vBottom, int t, std::vector<int>& triangles) {
	for (int i = 1; i < steps; i++) {
		triangles[t++] = vBottom;
		triangles[t++] = vTop - 1;
		triangles[t++] = vTop;

		triangles[t++] = vBottom++;
		triangles[t++] = vTop++;
		triangles[t++] = vBottom;
	}
	triangles[t++] = vBottom;
	triangles[t++] = vTop - 1;
	triangles[t++] = vTop;
	return t;
}

int COctaSphere::CreateUpperStrip(int steps, int vTop, int vBottom, int t, std::vector<int>& triangles) {
	triangles[t++] = vBottom;
	triangles[t++] = vTop - 1;
	triangles[t++] = ++vBottom;
	for (int i = 1; i <= steps; i++) {
		triangles[t++] = vTop - 1;
		triangles[t++] = vTop;
		triangles[t++] = vBottom;

		triangles[t++] = vBottom;
		triangles[t++] = vTop++;
		triangles[t++] = ++vBottom;
	}
	return t;
}

void COctaSphere::Normalize(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals) {
	for (int i = 0; i < vertices.size(); i++) {
		normals[i] = vertices[i] = glm::normalize(vertices[i]);
	}
}

void COctaSphere::CreateUV(std::vector<glm::vec3>& vertices, std::vector<int>& triangles, std::vector<glm::vec2>& uv) {
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec2 textureCoordinates{};
		textureCoordinates.x = 0.5f + glm::atan(vertices[i].x, vertices[i].z) / (-2.0f * glm::pi<float>());

		if (textureCoordinates.x < 0.0f) {
			textureCoordinates.x += 1.0f;
		}

		textureCoordinates.y = (glm::asin(vertices[i].y) / glm::pi<float>()) + 0.5f;
		uv[i] = textureCoordinates;
	}

	std::unordered_map<int, int> used{};
	int idx = vertices.size() - 1;

	// Fix wrapped UVs. Adapted from: http://mft-dev.dk/uv-mapping-sphere/
	for (int i = 0; i < triangles.size(); i += 3) {
		int index0 = triangles[i];
		int index1 = triangles[i + 1];
		int index2 = triangles[i + 2];

		// Find triangle seam
		glm::vec3 a{}, b{}, c{};
		b[2] = a[2] = 0;
		a[0] = uv[index1].x - uv[index0].x; // B-A
		a[1] = uv[index1].y - uv[index0].y;

		b[0] = uv[index2].x - uv[index0].x; // C-A
		b[1] = uv[index2].y - uv[index0].y;

		c = glm::cross(a, b); // B-A x C-A

		if (c[2] < 0) { // Is a triangle seam
			for (int j = 0; j < 3; ++j) {
				int ii = triangles[i + j];

				if (uv[ii].x < 0.25f) {
					if (used.find(ii) == used.end()) {
						vertices.push_back(vertices[ii]);
						uv.push_back(uv[ii]);
						uv[uv.size() - 1].x += 1;
						used[ii] = ++idx;
						triangles[i + j] = idx;
					}
					else {
						triangles[i + j] = used[ii];
					}
				}
			}
		}
	}

	// Find the North and South Poles of the sphere.
	float indexMin = 0, indexMax = 0, yMin = 1, yMax = -1;
	for (int i = 0; i < vertices.size(); ++i) {
		if (vertices[i].y > yMax) {
			yMax = vertices[i].y;
			indexMax = i;
		}
		if (vertices[i].y < yMin) {
			yMin = vertices[i].y;
			indexMin = i;
		}
	}

	for (int c = 0; c < 4; c++) {
		int iMin = indexMin + c;
		int iMax = indexMax + c;
		// Rearrange the faces to have the pole vertex at the zero position for the face.
		for (int i = 0; i < triangles.size(); i += 3) {
			if (triangles[i + 1] == iMax) {
				triangles[i + 1] = triangles[i + 2];
				triangles[i + 2] = triangles[i];
				triangles[i] = iMax;
			}

			if (triangles[i + 1] == iMin) {
				triangles[i + 1] = triangles[i + 2];
				triangles[i + 2] = triangles[i];
				triangles[i] = iMin;
			}

			if (triangles[i + 2] == iMax) {
				triangles[i + 2] = triangles[i + 1];
				triangles[i + 1] = triangles[i];
				triangles[i] = iMax;
			}

			if (triangles[i + 2] == iMin) {
				triangles[i + 2] = triangles[i + 1];
				triangles[i + 1] = triangles[i];
				triangles[i] = iMin;
			}
		}

		idx = vertices.size() - 1;
		for (int i = 0; i < triangles.size(); i += 3) {
			if (!(triangles[i] == iMin || triangles[i] == iMax)) {
				continue;
			}

			int index0 = triangles[i];
			int index1 = triangles[i + 1];
			int index2 = triangles[i + 2];

			vertices.push_back(vertices[index0]);
			uv.push_back(uv[index0]);
			uv[uv.size() - 1].x = (uv[index1].x + uv[index2].x) * 0.5f;
			triangles[i] = ++idx;
		}
	}
}

void COctaSphere::CreateTangents(std::vector<glm::vec3>& vertices, std::vector<glm::vec4>& tangents) {
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 v = vertices[i];
		v.y = 0.0f;
		v = glm::normalize(v);
		glm::vec4 tangent;
		tangent.x = -v.z;
		tangent.y = 0.0f;
		tangent.z = v.x;
		tangent.w = -1.0f;
		tangents[i] = tangent;
	}

	tangents[vertices.size() - 4] = tangents[0] = glm::normalize(glm::vec4(-1.0f, 0.0f, -1.0f, -1.0f));
	tangents[vertices.size() - 3] = tangents[1] = glm::normalize(glm::vec4(1.0f, 0.0f, -1.0f, -1.0f));
	tangents[vertices.size() - 2] = tangents[2] = glm::normalize(glm::vec4(1.0f, 0.0f, 1.0f, -1.0f));
	tangents[vertices.size() - 1] = tangents[3] = glm::normalize(glm::vec4(-1.0f, 0.0f, 1.0f, -1.0f));
	for (int i = 0; i < 4; i++) {
		tangents[vertices.size() - 1 - i].w = tangents[i].w = -1.0f;
	}
}

// Render the sphere as a set of triangles
void COctaSphere::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawElements(GL_TRIANGLES, m_triangles.size() * 3, GL_UNSIGNED_INT, 0);
}

// Release memory on the GPU
void COctaSphere::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}