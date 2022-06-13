#version 400 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
	// Output distance to light world position to depth buffer.
	gl_FragDepth = length(FragPos.xyz - lightPos) / farPlane;
}
