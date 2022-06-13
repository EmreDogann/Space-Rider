#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in mat4 inInstanceMatrix;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// used for skybox
out vec4 positionView;	// position of shadow being shaded in view space.
out vec3 normalView;	// normal of shadow being shaded in view space.

// This is the entry point into the vertex shader
void main() {
	worldPosition = inPosition;

	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * inInstanceMatrix * vec4(inPosition, 1.0f);
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vec4 vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	positionView = vEyePosition;
	normalView = vEyeNorm;

	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
	