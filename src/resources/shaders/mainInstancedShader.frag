#version 400 core

in vec3 worldPosition;
in vec4 positionView;	// position of shadow being shaded in view space.
in vec3 normalView;		// normal of shadow being shaded in view space.
in vec2 vTexCoord;		// Interpolated texture coordinate using texture coordinate from the vertex shader

layout(location = 0) out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	sampler2D emission;
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1; 
uniform MaterialInfo material1;

// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light1.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 h = normalize(v + s);
	vec3 n = eyeNorm;
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(h, n), 0.0f), material1.shininess + eps);
	
	vec3 emission = pow(texture(material1.emission, vTexCoord).rgb, vec3(2.2)) * 100.0;

	return ambient + diffuse + specular + emission;
}

void main() {
	// Get the texel colour from the texture sampler
	vec4 vTexColour = pow(texture(sampler0, vTexCoord), vec4(2.2)); // Gamme correct it back to linear space.
	vec3 vColour = PhongModel(positionView, normalize(normalView));

	if (bUseTexture) {
		vOutputColour = vTexColour * vec4(vColour, 1.0f);	// Combine object colour and texture 
	} else {
		vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead
	}
}