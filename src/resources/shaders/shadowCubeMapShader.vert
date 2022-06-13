#version 400 core

layout(location = 0) in vec3 inPosition;

uniform mat4 model;

void main() {
	// Transform vertices to world space.
	gl_Position = model * vec4(inPosition, 1.0);
} 
	