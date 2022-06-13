#version 400 core

out vec4 FragColor;
  
in vec2 vTexCoords;

uniform sampler2D scene;
uniform sampler2D bloomImage;

uniform float exposure;
uniform float gamma;
uniform float blackCrossfade;

void main() {
    vec3 hdrColor = texture(scene, vTexCoords).rgb;
    vec3 bloomColor = texture(bloomImage, vTexCoords).rgb;

    // Additive blending
    hdrColor += bloomColor;

    // Tone Mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    // Gamma Correction
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0) * (1 - blackCrossfade);
}  