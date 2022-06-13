#version 400 core

in vec4 positionView;	// position of shadow being shaded in view space.
in vec4 worldPosition;
in vec3 normal;
in vec3 normalView;		// normal of shadow being shaded in view space.
in vec2 vTexCoord;		// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform samplerCube skybox;
uniform vec3 cameraPos;

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;

	float constant;
    float linear;
    float quadratic;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1;
uniform LightInfo lightPlayer;
uniform MaterialInfo material1;

// Retrowave colors
const vec3 colors[] = vec3[](
vec3(130,1,207) / 255,
vec3(232,0,70) / 255,
vec3(130,0,204) / 255);

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(LightInfo light, vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 h = normalize(v + s);
	vec3 n = eyeNorm;
	vec3 ambient = light.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0f), material1.shininess + eps);
	
	return ambient + diffuse + specular;
}

vec3 CalculateLighting(LightInfo light) {
	return PhongModel(light, positionView, normalize(normalView));
}

vec4 ramp(vec3 cols[3], vec2 uv) {
    float x = uv.x * float(cols.length() - 1);
    return vec4(mix(cols[int(x)], cols[int(x) + 1], smoothstep(0.0, 1.0, fract(x))), 1.0);
}

void main() {
	vec3 vColour = CalculateLighting(light1);

	float lightDistance = length(lightPlayer.position - positionView);
	float attenuation = 1.0 / (lightPlayer.constant + lightPlayer.linear * lightDistance + lightPlayer.quadratic * (lightDistance * lightDistance)); 

	vColour += attenuation * CalculateLighting(lightPlayer);

	float size = 1.0/6.0;   // size of the tile
	float edge = size/22.0; // size of the edge
	float face_tone = 0.02; // 0.9 for the face of the tile
	float edge_tone = 0.6; // 0.5 for the edge

	vec2 uv = vTexCoord;
	uv.x = sign(edge - mod(uv.x, size-0.001)); // Vertical lines.
	uv.y = sign(edge*5.0 - mod(uv.y, size+5.0)); // Horizontal lines.

	vec4 gridColor = vec4(face_tone - sign(uv.x + uv.y + 2.0) * (face_tone - edge_tone));
	vec4 finalColor = vec4(vColour, 1.0) * ramp(colors,vTexCoord) * gridColor;

	// Environment Mapping
	vec3 I = normalize(worldPosition.xyz - cameraPos);
	vec3 R = reflect(I, normalize(normal));

	vOutputColour = finalColor + texture(skybox, R);
}
