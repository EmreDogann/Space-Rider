#version 400 core

uniform vec3 lineColor;

layout(location = 0) out vec4 vOutputColour; // The output colour

void main() {
	vOutputColour = vec4(lineColor, 1.0);
}