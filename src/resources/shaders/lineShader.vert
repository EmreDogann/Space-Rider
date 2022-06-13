#version 400 core

// Structure for matrices
uniform mat4 projMatrix;
uniform mat4 modelViewMatrix;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;

void main() {
	gl_Position = projMatrix * modelViewMatrix * vec4(inPosition, 1.0f);
} 
	