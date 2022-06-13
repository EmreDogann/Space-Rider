#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix;
	mat4 modelMatrix;
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

uniform float time;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec2 vTexCoord;	// Texture coordinate
out vec4 positionView;	// position of shadow being shaded in view space.
out vec4 worldPosition;
out vec3 normalView;	// normal of shadow being shaded in view space.
out vec3 normal;

// This is the entry point into the vertex shader
void main() {
	float frequency = 1.0/20.0;
	float amplitude = 4.5;
	vec3 newPosition = inPosition;
	newPosition.y += sin(-gl_VertexID * frequency + (time * 0.0025)) * amplitude;
	newPosition.x += sin(gl_VertexID * frequency/3.0 + (time * 0.0025)) * amplitude/3.0;

	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(newPosition, 1.0);
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vec4 vEyePosition = matrices.modelViewMatrix * vec4(newPosition, 1.0);
	
	positionView = vEyePosition;
	normalView = vEyeNorm;
	worldPosition = matrices.modelMatrix * vec4(newPosition, 1.0);
	normal = inNormal;

	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
	