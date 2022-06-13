#version 400 core

out vec2 vTexCoords;

void main() {
    // https://trass3r.github.io/coding/2019/09/11/bufferless-rendering.html
    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    vec2 texCoord = (position + 1) * 0.5;

    gl_Position = vec4(position, 0.0, 1.0);
    vTexCoords = texCoord;
}